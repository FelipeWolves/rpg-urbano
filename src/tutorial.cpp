#include "rpg/tutorial.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>
namespace rpg::presentation {
std::string encode_tutorial_completion(const TutorialCompletion& value){return std::string("RPG_TUTORIAL_COMPLETION 1\nstandard ")+(value.standard?"1":"0")+"\nclassic "+(value.classic?"1":"0")+"\nend\n";}
TutorialCompletion decode_tutorial_completion(std::string_view text){for(const auto candidate:{TutorialCompletion{},TutorialCompletion{true,false},TutorialCompletion{false,true},TutorialCompletion{true,true}}){if(text==encode_tutorial_completion(candidate)){return candidate;}}throw SaveError("Registro de tutorial invalido; arquivo preservado.");}
TutorialCompletion load_tutorial_completion(storage::Files& files,const storage::Keys& keys){storage::validate_keys(keys,false);const auto read=storage::bounded_read(files,keys.primary,256);if(read.state==storage::ReadState::Missing){return {};}if(read.state!=storage::ReadState::Ready){throw SaveError("Registro de tutorial ilegivel; arquivo preservado.");}return decode_tutorial_completion(read.bytes);}
TutorialCompletion record_tutorial_completion(storage::Files& files,const storage::Keys& keys,bool classic){auto current=load_tutorial_completion(files,keys);if(classic){current.classic=true;}else{current.standard=true;}const auto bytes=encode_tutorial_completion(current);if(!files.ensure_directory()||!files.write(keys.temporary,bytes)||!files.replace(keys.primary,keys.temporary)){throw SaveError("Conclusao nao gravada; tente novamente. Saves de outros modos preservados.");}return current;}
WaveVisualDefinition standard_tutorial_definition(){
 auto pilot=felipe_pilot_setup(true);auto ally=pilot.roster[0];ally.id="CHAR_FELIPE_JOVEM";ally.max_health=11500;ally.attack=1350;ally.speed=115;ally.stats.defense=950;
 auto kit=pilot.loadouts[0];kit.fighter_id=ally.id;
 std::vector<SkillDefinition> core;for(auto skill:pilot.rules.skills){if(skill.owner=="FELIPE"){skill.owner=ally.id;core.push_back(skill);}}
 Definition first{"MOB_TEMPESTADE_01",Team::B,8500,850,102};first.stats.defense=700;
 Definition second{"MOB_TEMPESTADE_02",Team::B,9200,920,118};second.stats.defense=740;
 Definition third{"MOB_TEMPESTADE_03",Team::B,10800,1050,96};third.stats.defense=820;
 std::vector<EncounterDefinition> waves;std::vector<std::vector<CharacterLoadout>> sets;
 for(int wave=0;wave<2;++wave){auto enemies=wave==0?std::vector<Definition>{first}:std::vector<Definition>{second,third};auto rules=team_reference_rules();rules.skills=core;std::vector<CharacterLoadout> loadouts{kit};
  for(const auto& enemy:enemies){EffectDefinition damage{EffectKind::Damage};EffectDefinition stat{enemy.id==second.id?EffectKind::AttackUp:EffectKind::SpeedDown,enemy.id==second.id?2000:1500,2};stat.self_target=enemy.id==second.id;
   auto effects=std::vector<EffectDefinition>{damage};if(enemy.id!=third.id){effects.push_back(stat);}const auto id=enemy.id+"-A1";SkillDefinition basic{id,enemy.id,0,TargetRule::Enemy,effects};basic.basic=true;rules.skills.push_back(basic);loadouts.push_back({enemy.id,{{1,"Impacto da tempestade",id,{}}}});
  }
  Status bleed{EffectKind::Bleed,2,TickPolicy::EveryTurn,0,675};bleed.source_id=ally.id;bleed.source_team=Team::A;bleed.debuff=true;
  // Initial preparation is an explicit fixture; it does not consume an action or bypass a later ACC roll.
  InitialRuntime initial{enemies.back().id,enemies.back().speed,{bleed},{{enemies.back().id+"-A1",0}}};
  waves.push_back({wave==0?"centelha":"rajada-trovao",enemies,rules,{initial}});sets.push_back(std::move(loadouts));
 }
 return {StageDefinition{"tutorial-bola-perdida",2,{ally},std::move(waves),CarryPolicy::PreserveHealth,ActionBudget{200,LimitOutcome::Draw},RuntimeCarry::WaveV2},std::move(sets)};
}
std::vector<std::string> TutorialSession::lesson()const{
 if(intro_){return {"A BOLA PERDIDA — TUTORIAL PADRAO","O jovem Lobo desce das nuvens. As manifestacoes da tempestade sao obstaculos de treino.","Selecione um alvo e use Garra do Asfalto (A1). O alvo didatico ja tem Sangramento; a regra da A1 permanece a mesma.","Recarga, cargas e debuffs seguem as regras normais. Nao ha defesa generica neste perfil."};}return notes_;
}
VisualResult TutorialSession::observe(VisualResult result){if(!result.accepted()){return result;}
 const auto s=session_.state();const auto wave=session_.wave_index();
 bool passive=false,special=false,slow=false;for(const auto& e:result.action.events){passive|=e.type==EventType::PassiveTriggered;special|=e.type==EventType::SkillUsed&&result.action.action_context&&result.action.action_context->skill=="FELIPE-A3";slow|=e.status_kind==EffectKind::SpeedDown;}
 if(lesson_==0){notes_={"A1 e efeitos","O Sangramento inicial atende a condicao de debuff da A1. Um novo Sangramento ainda depende dos testes normais de acerto/resistencia.","A3 — Eclipse na Floresta de Pedra: tres hits em area, dissipa ate dois buffs comuns e entra em recarga4."};lesson_=1;}
 if(passive){notes_={"Postura de Vigilia","A2 e passiva. A pre-carga4/5 permite demonstrar a reacao quando um ataque inimigo elegivel completar as cargas.","O contra-ataque e o escudo usam as mesmas regras do piloto. A passiva nao e um botao de acao."};lesson_=std::max<std::size_t>(lesson_,2);}
 if(special){notes_.push_back("A3 executada. Consulte a recarga e os efeitos atuais; uma habilidade em recarga nao esta pronta para nova acao.");}
 if(slow){notes_.push_back("SPD -15% altera o ganho de AP. O efeito dura2oportunidades naturais do portador; reacao fora de turno nao e oportunidade.");}
 if(wave==1&&lesson_<4){notes_={"ONDA2 — RAJADA E TROVAO","A transicao usa WaveV2: limpeza de efeitos comuns e cura30% do PV maximo, limitada ao PV maximo.","Rajada usa buff+20%ATK. Observe sua presenca antes de dissipar com A3. Trovao inicia com Sangramento para exercitar A1.","Escolha prioridade de alvo. O teto de200acoes encerra em empate, nao em vitoria."};lesson_=4;}
 if(session_.finished()){notes_={s.combat.battle.winner==Team::A?"O ceu comeca a abrir. O jovem Lobo segue em busca da bola.":"Tentativa encerrada. Consulte o resultado e tente novamente.","Conclusao em arquivo proprio do tutorial; sem moeda ou alteracao dos saves dos outros laboratorios."};}
 return result;
}
VisualResult TutorialSession::submit(std::uint64_t revision,std::uint32_t slot,std::string target){if(awaiting_lesson()){return {VisualError::NotPlayerTurn,{}};}if(!allowed_slot(slot)){return {VisualError::CoreRejected,ActionResult{ActionError::SkillConditionNotMet}};}return observe(session_.submit(revision,slot,std::move(target)));}
VisualResult TutorialSession::submit_action(std::uint64_t revision,const Action& action){if(awaiting_lesson()){return {VisualError::NotPlayerTurn,{}};}if(lesson_==0 && (action.type!=ActionType::Skill || action.skill!="FELIPE-A1")){return {VisualError::CoreRejected,ActionResult{ActionError::SkillConditionNotMet}};}return observe(session_.submit_action(revision,action));}
VisualResult TutorialSession::step_automatic(){if(awaiting_lesson()){return {VisualError::NotPlayerTurn,{}};}return observe(session_.step_automatic());}
ClassicSearchSession::ClassicSearchSession(SearchSettings settings):settings_(settings){if(settings.search_gain<1||settings.search_gain>100||settings.concentration_bonus<1||settings.concentration_bonus>100||settings.storm_gain<1||settings.storm_gain>100||settings.storm_reduction<0||settings.storm_reduction>100||settings.action_limit<1||settings.action_limit>10000){throw std::invalid_argument("Invalid search settings");}}
bool ClassicSearchSession::act(std::uint64_t revision,SearchAction action){if(revision!=state_.revision||state_.dialogue||state_.outcome!=SearchOutcome::Running){return false;}if(action!=SearchAction::Reconnaissance&&action!=SearchAction::Concentrate&&action!=SearchAction::Advance){return false;}
 if(state_.revision==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Search revision exhausted");}
 if(action==SearchAction::Reconnaissance){state_.search=std::min(100,state_.search+settings_.search_gain+(state_.concentrated?settings_.concentration_bonus:0));state_.concentrated=false;}else if(action==SearchAction::Concentrate){state_.concentrated=true;}else{state_.storm=std::max(0,state_.storm-settings_.storm_reduction);}
 ++state_.actions;++state_.revision;state_.storm=std::min(100,state_.storm+settings_.storm_gain);
 if(state_.search==100){state_.outcome=SearchOutcome::Recovered;}else if(state_.storm==100){state_.outcome=SearchOutcome::Storm;}else if(state_.actions>=settings_.action_limit){state_.outcome=SearchOutcome::ActionLimit;}
 if(state_.outcome==SearchOutcome::Running&&state_.search>=50&&!state_.dialogue_seen){state_.dialogue=true;state_.dialogue_seen=true;}return true;
}
bool ClassicSearchSession::acknowledge(){if(!state_.dialogue){return false;}state_.dialogue=false;return true;}
void ClassicSearchSession::reset(){if(state_.revision==std::numeric_limits<std::uint64_t>::max()||state_.attempt==std::numeric_limits<std::uint64_t>::max()){throw std::overflow_error("Search reset exhausted");}const auto revision=state_.revision+1,attempt=state_.attempt+1;state_={};state_.revision=revision;state_.attempt=attempt;}
void ClassicSearchSession::abandon(){if(state_.outcome==SearchOutcome::Running){state_.outcome=SearchOutcome::Abandoned;state_.dialogue=false;}}
std::string hub_uppercase(std::string text){for(char& c:text){if(c>='a'&&c<='z'){c=static_cast<char>(c-32);}}for(const auto& pair:{std::pair{"ç","Ç"},std::pair{"ã","Ã"},std::pair{"â","Â"},std::pair{"à","À"},std::pair{"õ","Õ"},std::pair{"ü","Ü"},std::pair{"á","Á"},std::pair{"é","É"},std::pair{"ó","Ó"},std::pair{"í","Í"},std::pair{"ú","Ú"},std::pair{"ê","Ê"},std::pair{"ô","Ô"}}){std::size_t pos=0;while((pos=text.find(pair.first,pos))!=std::string::npos){text.replace(pos,std::char_traits<char>::length(pair.first),pair.second);pos+=std::char_traits<char>::length(pair.second);}}return text;}
}
