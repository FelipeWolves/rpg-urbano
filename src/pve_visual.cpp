#include "rpg/pve_visual.hpp"
#include "rpg/battle_presentation.hpp"
#include <limits>
#include <stdexcept>
namespace rpg::presentation {
PveDefinition ritual_lab_definition(){
 auto setup=felipe_pilot_setup();PveDefinition d;d.id="ritual-lab";d.attempt_id="ritual-lab-1";d.boss_id="ECO-1";
 d.roster=std::move(setup.roster);d.rules=std::move(setup.rules);d.loadouts=std::move(setup.loadouts);
 for(auto& f:d.roster){if(f.id=="ECO-2"){f.role=EntityRole::Auxiliary;}}
 return d;
}
PveDefinition phased_ritual_lab_definition(){
 auto d=ritual_lab_definition();d.id="ritual-phases-lab";d.attempt_id="ritual-phases-1";
 const auto skill=std::ranges::find(d.rules.skills,std::string("ECO-1-A1"),&SkillDefinition::id);
 PhaseMutation fury;fury.kit={*skill};fury.kit[0].effects[0].power_ratio=15000;
 d.phases={{"Vigilia-90",PvePhaseTrigger::HealthThreshold,9000,{}},{"Furia-60",PvePhaseTrigger::HealthThreshold,6000,std::move(fury)}};
 return d;
}
PveDefinition score_lab_definition(){
 auto d=ritual_lab_definition();d.id="score-lab";d.attempt_id="score-lab-1";d.mode=PveMode::ScoreAttack;d.rules.action_budget=ActionBudget{12,LimitOutcome::Draw};
 auto boss=std::ranges::find(d.roster,d.boss_id,&Definition::id);boss->max_health=1000000;return d;
}
PveDefinition survival_lab_definition(){
 auto d=ritual_lab_definition();d.id="survival-lab";d.attempt_id="survival-lab-1";d.mode=PveMode::Survival;d.boss_turn_goal=3;d.boss_turn_clock=TickPolicy::NaturalOnly;d.score_enabled=true;
 auto boss=std::ranges::find(d.roster,d.boss_id,&Definition::id);boss->max_health=1000000;return d;
}
PveDefinition pve_lab_definition(PveLabKind kind){
 switch(kind){case PveLabKind::Ritual:return ritual_lab_definition();case PveLabKind::Phased:return phased_ritual_lab_definition();case PveLabKind::Score:return score_lab_definition();case PveLabKind::Survival:return survival_lab_definition();}
 throw std::invalid_argument("Unknown local PvE variant");
}
std::optional<ScoreReceipt> PveVisualSession::receipt()const{
 if(!finished()){return std::nullopt;}
 const auto id=encounter_->snapshot().attempt_id;const auto& receipts=ledger_->receipts();const auto found=std::ranges::find(receipts,id,&ScoreReceipt::attempt_id);
 return found==receipts.end()?std::nullopt:std::optional<ScoreReceipt>{*found};
}
PveVisualSession::PveVisualSession(PveDefinition definition):definition_(std::move(definition)),ledger_(std::make_unique<ScoreLedger>()),encounter_(std::make_unique<PveEncounter>(definition_,*ledger_)){
 if(definition_.loadouts.empty()){throw std::invalid_argument("Visual PvE requires declared team slots");}
}
VisualState PveVisualSession::state()const{
 auto snapshot=encounter_->snapshot();const auto* actor=encounter_->battle().current_actor();
 return {revision_,{CombatProfile::Team,encounter_->loadouts(),std::move(snapshot.battle),{}},{attempt_,0,{}},actor?encounter_->abilities(actor->definition.id):std::vector<AbilityView>{},
  !finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
void PveVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("PvE revision exhausted");}}
VisualResult PveVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}
 const auto* actor=encounter_->battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult PveVisualSession::accept(PveActionResult result,bool automatic){
 if(!result.action.accepted()){return {VisualError::CoreRejected,std::move(result.action)};}
 if(!result.phase_events.empty()){phase_events_=std::move(result.phase_events);}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult PveVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();return accept(encounter_->apply_slot(slot,std::move(target)),false);
}
VisualResult PveVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();return accept(encounter_->apply(action),false);
}
VisualResult PveVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}
 check_capacity();const auto action=encounter_->choose_automatic_action();if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}
 auto result=accept(encounter_->apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void PveVisualSession::reset(){
 check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("PvE attempt exhausted");}
 auto next=definition_;next.attempt_id=definition_.attempt_id+"-retry-"+std::to_string(attempt_+1);
 auto replacement=std::make_unique<PveEncounter>(std::move(next),*ledger_);encounter_=std::move(replacement);++attempt_;++revision_;automatic_count_=0;stopped_=false;cues_.clear();phase_events_.clear();
}
namespace {
WaveVisualDefinition validate_waves(WaveVisualDefinition d){
 if(d.loadouts.size()!=d.stage.encounters.size()){throw std::invalid_argument("Wave loadouts must match encounters");}
 for(std::size_t i=0;i<d.loadouts.size();++i){
  auto roster=d.stage.squad;roster.insert(roster.end(),d.stage.encounters[i].enemies.begin(),d.stage.encounters[i].enemies.end());
  for(const auto& set:d.loadouts[i]){for(const auto& entry:set.slots){if(entry.passive&&entry.passive->trigger==PassiveTrigger::BattleStart){throw std::invalid_argument("Wave opening passives require explicit stage runtime initialization");}}}
  ProfileBattle profile(roster,d.stage.encounters[i].rules,CombatProfile::Team,d.loadouts[i]);
  d.stage.encounters[i].rules=profile.snapshot().battle.rules;d.loadouts[i]=profile.snapshot().loadouts;
 }
 return d;
}
}
WaveVisualDefinition wave_lab_definition(){
 const auto source=felipe_pilot_setup();const auto ally=*std::ranges::find(source.roster,std::string("FELIPE"),&Definition::id);
 const auto ally_set=*std::ranges::find(source.loadouts,std::string("FELIPE"),&CharacterLoadout::fighter_id);
 std::vector<EncounterDefinition> waves;std::vector<std::vector<CharacterLoadout>> sets;
 for(const auto& id:{std::string("ECO-1"),std::string("ECO-2")}){
  const auto enemy=*std::ranges::find(source.roster,id,&Definition::id);auto rules=source.rules;
  std::erase_if(rules.skills,[&](const SkillDefinition& s){return s.owner!="FELIPE"&&s.owner!=id;});
  waves.push_back({"onda-"+id,{enemy},std::move(rules)});
  sets.push_back({ally_set,*std::ranges::find(source.loadouts,id,&CharacterLoadout::fighter_id)});
 }
 return {StageDefinition{"waves-lab",2,{ally},std::move(waves),CarryPolicy::PreserveHealth,ActionBudget{200,LimitOutcome::Draw},RuntimeCarry::WaveV2},std::move(sets)};
}
WaveVisualSession::WaveVisualSession(WaveVisualDefinition definition):definition_(validate_waves(std::move(definition))),stage_(definition_.stage){}
VisualState WaveVisualSession::state()const{
 const auto* actor=stage_.battle().current_actor();return {revision_,{CombatProfile::Team,loadouts(),stage_.battle().snapshot(),{}},{attempt_,0,{}},actor?team_abilities(stage_.battle(),loadouts(),actor->definition.id):std::vector<AbilityView>{},!finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
WaveHud WaveVisualSession::hud()const{
 const auto s=stage_.snapshot();std::string status="Em andamento";
 if(s.end_reason){switch(*s.end_reason){case StageEndReason::Completed:status="Todas as ondas concluidas";break;case StageEndReason::GlobalActionLimit:status="Limite global de acoes atingido";break;case StageEndReason::TechnicalLimit:status="Interrupcao por limite tecnico";break;case StageEndReason::NoSurvivors:status="Sem sobreviventes";break;case StageEndReason::EncounterFailure:status="Encontro encerrado sem completar as ondas";break;}}
 return {s.stage_id,s.encounter_id,std::move(status),s.encounter_index+1,definition_.stage.encounters.size(),s.accepted_actions,definition_.stage.global_budget?std::optional<std::uint64_t>{static_cast<std::uint64_t>(definition_.stage.global_budget->max_actions)}:std::nullopt,stage_.remaining_actions(),s.battle.accepted_actions,stage_.battle().remaining_actions(),finished(),s.end_reason==StageEndReason::Completed,previous_wave_};
}
void WaveVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Wave revision exhausted");}}
VisualResult WaveVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}
 const auto* actor=stage_.battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult WaveVisualSession::accept(StageActionResult result,bool automatic){
 if(!result.action.accepted()){return {VisualError::CoreRejected,std::move(result.action)};}
 if(result.encounter_advanced){previous_wave_=result.encounter_id;transition_events_=std::move(result.transition_events);}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult WaveVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_slot(stage_.battle(),loadouts(),slot,std::move(target));if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(stage_.apply(*prepared.action),false);
}
VisualResult WaveVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_action(stage_.battle(),loadouts(),action);if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(stage_.apply(*prepared.action),false);
}
VisualResult WaveVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}
 check_capacity();const auto action=choose_team_action(stage_.battle(),loadouts());if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}auto result=accept(stage_.apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void WaveVisualSession::reset(){check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Wave attempt exhausted");}stage_.reset();++revision_;++attempt_;automatic_count_=0;stopped_=false;cues_.clear();transition_events_.clear();previous_wave_.reset();}namespace {
SequentialVisualDefinition validate_sequence(SequentialVisualDefinition d){
 if(d.encounter.squads.size()!=3 || d.loadouts.size()!=3 || (!d.encounter.boss_phases.empty() && d.encounter.boss_phases.size()!=3)){throw std::invalid_argument("Visual sequence requires three catalogs/squads");}
 for(std::size_t i=0;i<3;++i){
  auto& squad=d.encounter.squads[i];std::vector<Definition> roster;for(const auto& member:squad.members){roster.push_back(member.fighter);}roster.push_back(d.encounter.boss);
  for(const auto& set:d.loadouts[i]){for(const auto& entry:set.slots){if(entry.passive&&entry.passive->trigger==PassiveTrigger::BattleStart){throw std::invalid_argument("Sequential opening passives require explicit runtime adapter");}}}
  auto rules=squad.rules;if(!d.encounter.boss_phases.empty()){const auto& kit=d.encounter.boss_phases[i].skills;rules.skills.insert(rules.skills.end(),kit.begin(),kit.end());}
  ProfileBattle profile(roster,rules,CombatProfile::Team,d.loadouts[i]);const auto view=profile.snapshot();squad.rules=view.battle.rules;d.loadouts[i]=view.loadouts;
  if(!d.encounter.boss_phases.empty()){
   std::vector<SkillDefinition> kit;for(const auto& skill:squad.rules.skills){if(skill.owner==d.encounter.boss.id){kit.push_back(skill);}}d.encounter.boss_phases[i].skills=std::move(kit);
   std::erase_if(squad.rules.skills,[&](const SkillDefinition& skill){return skill.owner==d.encounter.boss.id;});
  }
 }
 return d;
}
}
SequentialVisualDefinition sequential_lab_definition(bool phases){
 const auto pilot=felipe_pilot_setup(),generic=terminal_lab_setup();
 auto boss=*std::ranges::find(pilot.roster,std::string("ECO-1"),&Definition::id);boss.id="CHEFE-SEQ";boss.max_health=12500;boss.speed=10;boss.role=EntityRole::Boss;boss.is_boss=true;
 auto boss_skill=*std::ranges::find(pilot.rules.skills,std::string("ECO-1-A1"),&SkillDefinition::id);boss_skill.id="CHEFE-SEQ-A1";boss_skill.owner=boss.id;
 CharacterLoadout boss_set{boss.id,{{1,"Pulso do chefe",boss_skill.id,""}}};
 std::vector<SquadDefinition> squads;std::vector<std::vector<CharacterLoadout>> sets;std::vector<BossPhaseDefinition> kits;
 const std::vector<std::string> ids={"FELIPE","A1","B1"};
 for(std::size_t i=0;i<ids.size();++i){
  const auto& source=i==0?pilot:generic;auto member=*std::ranges::find(source.roster,ids[i],&Definition::id);member.team=Team::A;
  auto rules=team_reference_rules();rules.action_budget=ActionBudget{4,LimitOutcome::Draw};for(const auto& skill:source.rules.skills){if(skill.owner==member.id){rules.skills.push_back(skill);}}
  auto kit=boss_skill;if(phases){kit.effects[0].power_ratio=10000+static_cast<numeric::BasisPoints>(i)*2500;kits.emplace_back("SEQ-"+std::to_string(i+1),std::vector<SkillDefinition>{kit},BossCooldownTransition::KeepMatching);}else{rules.skills.push_back(kit);}
  squads.push_back({"equipe-"+std::to_string(i+1),{{member.id,member}},std::move(rules)});
  sets.push_back({*std::ranges::find(source.loadouts,member.id,&CharacterLoadout::fighter_id),boss_set});
 }
 return {SequentialDefinition{phases?"seq-phases-lab":"seq-fixed-lab",2,std::move(squads),boss,BossRuntimeCarry::PreserveSpecialBordersV2,ActionBudget{200,LimitOutcome::Draw},std::move(kits)},std::move(sets)};
}
SequentialVisualSession::SequentialVisualSession(SequentialVisualDefinition definition):definition_(validate_sequence(std::move(definition))),encounter_(definition_.encounter){}
VisualState SequentialVisualSession::state()const{
 const auto* actor=encounter_.battle().current_actor();return {revision_,{CombatProfile::Team,loadouts(),encounter_.battle().snapshot(),{}},{attempt_,0,{}},actor?team_abilities(encounter_.battle(),loadouts(),actor->definition.id):std::vector<AbilityView>{},!finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
SequentialHud SequentialVisualSession::hud()const{
 const auto s=encounter_.snapshot();const auto* boss=encounter_.battle().find(s.boss_id);std::string status="Em andamento";
 if(s.end_reason){switch(*s.end_reason){case SequentialEndReason::BossDefeated:status="Chefe derrotado";break;case SequentialEndReason::SquadsExhausted:status="Equipes esgotadas";break;case SequentialEndReason::GlobalActionLimit:status="Limite global de acoes atingido";break;case SequentialEndReason::TechnicalLimit:status="Interrupcao por limite tecnico";break;case SequentialEndReason::MutualElimination:status="Eliminacao mutua; resultado definido pelo nucleo";break;}}
 const auto local=s.battle.rules.action_budget?std::optional<std::uint64_t>{static_cast<std::uint64_t>(s.battle.rules.action_budget->max_actions)}:std::nullopt;
 const auto global=definition_.encounter.global_budget?std::optional<std::uint64_t>{static_cast<std::uint64_t>(definition_.encounter.global_budget->max_actions)}:std::nullopt;
 const auto next=!finished()&&s.squad_index+1<definition_.encounter.squads.size()?std::optional<std::string>{definition_.encounter.squads[s.squad_index+1].id}:std::nullopt;
 return {s.stage_id,s.squad_id,s.boss_id,s.boss_phase_id,std::move(status),s.squad_index+1,definition_.encounter.squads.size(),boss->health,boss->definition.max_health,s.accepted_actions,s.battle.accepted_actions,global,encounter_.remaining_actions(),local,encounter_.battle().remaining_actions(),next,previous_squad_,previous_phase_,s.boss_carry,finished(),s.end_reason==SequentialEndReason::BossDefeated};
}
void SequentialVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Sequence revision exhausted");}}
VisualResult SequentialVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}const auto* actor=encounter_.battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult SequentialVisualSession::accept(SequentialActionResult result,bool automatic){
 if(!result.action.accepted()){return {VisualError::CoreRejected,std::move(result.action)};}if(result.squad_advanced){previous_squad_=result.squad_id;previous_phase_=result.boss_phase_id;}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult SequentialVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_slot(encounter_.battle(),loadouts(),slot,std::move(target));if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(encounter_.apply(*prepared.action),false);
}
VisualResult SequentialVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_action(encounter_.battle(),loadouts(),action);if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(encounter_.apply(*prepared.action),false);
}
VisualResult SequentialVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}check_capacity();const auto action=choose_team_action(encounter_.battle(),loadouts());if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}auto result=accept(encounter_.apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void SequentialVisualSession::reset(){check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Sequence attempt exhausted");}encounter_.reset();++revision_;++attempt_;automatic_count_=0;stopped_=false;cues_.clear();previous_squad_.reset();previous_phase_.reset();}namespace {
SharedVisualDefinition validate_shared(SharedVisualDefinition d){
 if(d.encounter.config_version!=2 || d.encounter.squads.size()!=3 || !d.encounter.front_enemies.empty()){throw std::invalid_argument("Visual shared profile requires v2, three fronts, no auxiliary layout yet");}
 std::vector<Definition> roster;for(const auto& squad:d.encounter.squads){for(const auto& member:squad.members){roster.push_back(member.fighter);}}roster.push_back(d.encounter.boss);
 for(const auto& set:d.loadouts){for(const auto& slot:set.slots){if(slot.passive&&slot.passive->trigger==PassiveTrigger::BattleStart){throw std::invalid_argument("Shared opening passives require explicit runtime adapter");}}}
 ProfileBattle profile(roster,d.encounter.rules,CombatProfile::Team,d.loadouts);d.encounter.rules=profile.snapshot().battle.rules;d.loadouts=profile.snapshot().loadouts;return d;
}
std::optional<std::size_t> shared_front_of(const SharedBossSnapshot& s,std::string_view id){for(std::size_t i=0;i<s.members.size();++i){if(std::ranges::find(s.members[i],id)!=s.members[i].end()){return i;}}return std::nullopt;}
}
SharedVisualDefinition shared_lab_definition(bool global_special){
 const auto sequential=sequential_lab_definition(false);SharedBossDefinition d;d.id=global_special?"shared-global-lab":"shared-local-lab";d.config_version=2;d.boss=sequential.encounter.boss;d.boss.id="CHEFE-PAR";d.boss.max_health=150000;d.boss.attack=700;
 d.rules=team_reference_rules();std::vector<CharacterLoadout> sets;
 for(std::size_t i=0;i<sequential.encounter.squads.size();++i){
  const auto& squad=sequential.encounter.squads[i];const auto& member=squad.members[0];d.squads.push_back({"frente-"+std::to_string(i+1),{{member.character_id,member.fighter}}});
  for(const auto& skill:squad.rules.skills){if(skill.owner==member.fighter.id){d.rules.skills.push_back(skill);d.skill_scopes.push_back({skill.id,skill.target==TargetRule::Self?SharedSkillTarget::Self:skill.target==TargetRule::Ally?SharedSkillTarget::OwnSquad:SharedSkillTarget::Boss});}}
  sets.push_back(*std::ranges::find(sequential.loadouts[i],member.fighter.id,&CharacterLoadout::fighter_id));
 }
 SkillDefinition basic{"CHEFE-PAR-A1",d.boss.id,0,TargetRule::Enemy,{{EffectKind::Damage}}};basic.basic=true;d.rules.skills.push_back(basic);d.skill_scopes.push_back({basic.id,SharedSkillTarget::RotatingFront});
 CharacterLoadout boss{d.boss.id,{{1,"Pulso local",basic.id,"Ataque comum alterna entre frentes vivas."}}};
 if(global_special){SkillDefinition special{"CHEFE-PAR-A2",d.boss.id,3,TargetRule::EnemyTeam,{{EffectKind::Damage}}};d.rules.skills.push_back(special);d.skill_scopes.push_back({special.id,SharedSkillTarget::AllFronts});boss.slots.push_back({2,"Pulso global",special.id,"Ataque especial declarado para todas as frentes."});boss.automatic_priority={2};}
 sets.push_back(std::move(boss));return {std::move(d),std::move(sets)};
}
SharedVisualSession::SharedVisualSession(SharedVisualDefinition definition):definition_(validate_shared(std::move(definition))),encounter_(definition_.encounter),front_actions_(3,0){}
VisualState SharedVisualSession::state()const{
 const auto* actor=encounter_.battle().current_actor();return {revision_,{CombatProfile::Team,definition_.loadouts,encounter_.battle().snapshot(),{}},{attempt_,0,{}},actor?team_abilities(encounter_.battle(),definition_.loadouts,actor->definition.id):std::vector<AbilityView>{},!finished()&&actor&&actor->definition.team==Team::B,stopped_||finished()};
}
SharedHud SharedVisualSession::hud()const{
 const auto s=encounter_.snapshot();const auto* boss=encounter_.battle().find(s.boss_id);std::string status="Em andamento";
 const bool objective=finished()&&s.battle.winner==Team::A&&boss->health==0&&s.battle.end_reason==EndReason::Elimination;
 if(finished()){status=objective?"Chefe compartilhado derrotado":s.battle.end_reason==EndReason::ActionLimit?"Limite global de acoes atingido":s.battle.end_reason==EndReason::TechnicalLimit?"Interrupcao por limite tecnico":"Encontro encerrado sem cumprir objetivo";}
 std::vector<SharedFrontHud> fronts;for(std::size_t i=0;i<s.squad_ids.size();++i){fronts.push_back({s.squad_ids[i],s.active_squads[i],front_actions_[i],s.members[i]});}
 const auto index=s.battle.current_actor?shared_front_of(s,*s.battle.current_actor):std::nullopt;const auto actor_front=index?std::optional<std::string>{s.squad_ids[*index]}:std::nullopt;
 const auto goal=s.battle.rules.action_budget?std::optional<std::uint64_t>{static_cast<std::uint64_t>(s.battle.rules.action_budget->max_actions)}:std::nullopt;
 return {s.stage_id,s.boss_id,std::move(status),boss->health,boss->definition.max_health,s.battle.accepted_actions,boss_actions_,goal,encounter_.battle().remaining_actions(),std::move(fronts),actor_front,s.next_boss_front,last_local_front_,finished(),objective};
}
std::vector<std::string> SharedVisualSession::targets(std::uint32_t position)const{
 auto candidates=target_candidates(state(),position);const auto s=encounter_.snapshot();if(!s.battle.current_actor){return {};}
 const auto actor=*s.battle.current_actor;const auto set=std::ranges::find(definition_.loadouts,actor,&CharacterLoadout::fighter_id);if(set==definition_.loadouts.end()){return {};}
 const auto slot=std::ranges::find(set->slots,position,&AbilitySlot::position);if(slot==set->slots.end()||!slot->active_skill){return {};}
 const auto scope=std::ranges::find(s.skill_scopes,*slot->active_skill,&SharedSkillScope::skill_id);if(scope==s.skill_scopes.end()){return {};}
 const auto owner_front=shared_front_of(s,actor);
 std::erase_if(candidates,[&](const std::string& id){switch(scope->target){case SharedSkillTarget::Self:return id!=actor;case SharedSkillTarget::OwnSquad:return shared_front_of(s,id)!=owner_front;case SharedSkillTarget::Boss:return id!=s.boss_id;case SharedSkillTarget::RotatingFront:{const auto target=shared_front_of(s,id);return !target||!s.next_boss_front||s.squad_ids[*target]!=*s.next_boss_front;}case SharedSkillTarget::AllFronts:return !shared_front_of(s,id);}return true;});return candidates;
}
void SharedVisualSession::check_capacity()const{if(revision_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Shared revision exhausted");}}
VisualResult SharedVisualSession::preflight(std::uint64_t revision,bool automatic)const{
 if(revision!=revision_){return {VisualError::StaleCommand,{}};}if(finished()){return {VisualError::Finished,{}};}if(stopped_){return {VisualError::SafetyStop,{}};}const auto* actor=encounter_.battle().current_actor();if(!actor||(actor->definition.team==Team::B)!=automatic){return {VisualError::NotPlayerTurn,{}};}return {};
}
VisualResult SharedVisualSession::accept(SharedBossActionResult result,bool automatic){
 if(!result.accepted()){if(result.error==SharedBossError::WrongFront){result.action.error=ActionError::InvalidSkillTarget;}return {VisualError::CoreRejected,std::move(result.action)};}
 const auto s=encounter_.snapshot();const auto actor=result.action.action_context?result.action.action_context->actor:std::string{};const auto front=shared_front_of(s,actor);
 if(front){++front_actions_[*front];}else if(actor==s.boss_id){++boss_actions_;if(result.squad_id){last_local_front_=result.squad_id;}}
 ++revision_;automatic_count_=automatic?automatic_count_+1:0;static_cast<void>(cues_.observe(revision_,result.action));return {VisualError::None,std::move(result.action)};
}
VisualResult SharedVisualSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_slot(encounter_.battle(),definition_.loadouts,slot,std::move(target));if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(encounter_.apply(*prepared.action),false);
}
VisualResult SharedVisualSession::submit_action(std::uint64_t revision,const Action& action){
 const auto check=preflight(revision,false);if(!check.accepted()){return check;}check_capacity();auto prepared=prepare_team_action(encounter_.battle(),definition_.loadouts,action);if(!prepared.action){return {VisualError::CoreRejected,ActionResult{prepared.error}};}return accept(encounter_.apply(*prepared.action),false);
}
VisualResult SharedVisualSession::step_automatic(){
 const auto check=preflight(revision_,true);if(!check.accepted()){return check;}if(automatic_count_>=256){stopped_=true;return {VisualError::SafetyStop,{}};}check_capacity();auto action=choose_team_action(encounter_.battle(),definition_.loadouts);if(!action){stopped_=true;return {VisualError::SafetyStop,{}};}
 const auto scope=std::ranges::find(definition_.encounter.skill_scopes,action->skill,&SharedSkillScope::skill_id);
 if(scope!=definition_.encounter.skill_scopes.end()&&scope->target==SharedSkillTarget::RotatingFront){const auto routed=encounter_.choose_basic_action();if(!routed){stopped_=true;return {VisualError::SafetyStop,{}};}action->target=routed->target;}
 auto result=accept(encounter_.apply(*action),true);if(!result.accepted()){stopped_=true;}return result;
}
void SharedVisualSession::reset(){check_capacity();if(attempt_==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Shared attempt exhausted");}encounter_.reset();++revision_;++attempt_;boss_actions_=0;automatic_count_=0;stopped_=false;cues_.clear();front_actions_.assign(3,0);last_local_front_.reset();}}