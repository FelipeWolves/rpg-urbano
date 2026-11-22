#include "rpg/battle_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool ok,const char* why){if(!ok){throw std::runtime_error(why);}}
PveDefinition fixture(){
 PveDefinition d;d.id="ritual-lab";d.attempt_id="hud-attempt";d.boss_id="B";
 d.roster={{"A",Team::A,1000,100,100},{"B",Team::B,1000,10,90},{"ADD",Team::B,1000,1,10}};
 d.roster[2].role=EntityRole::Auxiliary;for(auto& f:d.roster){f.stats.block=0;}
 d.rules=team_reference_rules();return d;
}
void purity(){ScoreLedger ledger;PveEncounter e(fixture(),ledger);const auto before=e.snapshot();
 const auto first=pve_hud(before);for(int i=0;i<100;++i){check(pve_hud(e.snapshot())==first,"stable read");}
 check(e.snapshot()==before&&ledger.receipts().empty(),"HUD cannot advance RNG/clock/reward");
 auto copy=e.snapshot();copy.battle.fighters[0].health=1;check(e.snapshot()==before,"snapshot owns data");
 check(!first.phase_id&&!first.score&&first.commands_open&&!first.encounter_finished,"no fake phase or score");
}
void clocks(){ScoreLedger ledger;auto d=fixture();d.mode=PveMode::Survival;d.boss_turn_goal=3;d.rules.action_budget=ActionBudget{8,LimitOutcome::Draw};PveEncounter e(d,ledger);
 check(e.apply({"A",ActionType::Attack,"B"}).action.accepted(),"clock player action");
 auto h=pve_hud(e.snapshot());check(h.actions.completed==1&&h.actions.goal==8&&h.remaining_actions==7&&h.boss_turns.completed==0&&h.boss_turns.goal==3,"separate action/boss clocks");
 check(e.apply({"B",ActionType::Attack,"A"}).action.accepted(),"clock boss action");h=pve_hud(e.snapshot());check(h.actions.completed==2&&h.boss_turns.completed==1&&h.boss_turn_clock==TickPolicy::NaturalOnly,"authoritative boss clock");
}
void ritual(){ScoreLedger ledger;auto d=fixture();d.roster[1].max_health=100;PveEncounter e(d,ledger);
 check(e.apply({"A",ActionType::Attack,"B"}).objective_met,"ritual objective");const auto h=pve_hud(e.snapshot());
 check(h.encounter_finished&&h.objective_met&&!h.commands_open&&h.boss_health==0&&h.end==PveEnd::BossDefeated,"ritual HUD completion");
 check(e.battle().find("ADD")->health>0&&ledger.receipts().empty(),"adds do not invent reward or prevent boss objective");
}
void score_window(){ScoreLedger ledger;auto d=fixture();d.mode=PveMode::ScoreAttack;d.rules.action_budget=ActionBudget{1,LimitOutcome::DefeatA};PveEncounter e(d,ledger);
 check(e.apply({"A",ActionType::Attack,"B"}).action.accepted(),"score hit");auto h=pve_hud(e.snapshot());
 check(h.end==PveEnd::WindowClosed&&h.encounter_finished&&!h.objective_met&&!h.commands_open&&h.score==100,"score end not ritual win");
 check(h.status_label=="Janela de pontuacao encerrada"&&ledger.receipts().size()==1,"score result presentation");
 for(int i=0;i<10;++i){static_cast<void>(pve_hud(e.snapshot()));}check(ledger.receipts().size()==1,"reading cannot settle twice");
}
void survival(){ScoreLedger ledger;auto d=fixture();d.mode=PveMode::Survival;d.boss_turn_goal=1;PveEncounter e(d,ledger);
 check(e.apply({"A",ActionType::Attack,"B"}).action.accepted(),"survival player");check(e.apply({"B",ActionType::Attack,"A"}).objective_met,"survival reached");
 const auto s=e.snapshot();auto h=pve_hud(s);check(s.battle.phase==Phase::Running&&s.finished&&h.encounter_finished&&!h.commands_open&&h.objective_met,"outer encounter controls HUD termination");
}
void rejection(){ScoreLedger ledger;PveEncounter e(fixture(),ledger);const auto before=e.snapshot();
 check(!e.apply({"A",ActionType::Attack,"absent"}).action.accepted()&&e.snapshot()==before,"rejected command cannot change HUD");
}
void phases(){ScoreLedger ledger;auto d=fixture();d.roster[0].attack=700;d.phases={{"phase-70",PvePhaseTrigger::HealthThreshold,7000,{}},{"phase-35",PvePhaseTrigger::HealthThreshold,3500,{}}};PveEncounter e(d,ledger);
 check(!e.snapshot().phase_id,"no fabricated initial phase");auto r=e.apply({"A",ActionType::Attack,"B"});check(r.phase_events.size()==2&&pve_hud(e.snapshot()).phase_id=="phase-35","last completed phase observed");
 check(e.snapshot().battle.accepted_actions==1,"phase display creates no action");
}
void cycles(){ScoreLedger ledger;auto d=fixture();d.turn_cycle=PveTurnCycle{1,{{"cycle-one",{}},{"cycle-two",{}}}};PveEncounter e(d,ledger);
 check(e.apply({"A",ActionType::Attack,"B"}).action.accepted(),"cycle player");auto r=e.apply({"B",ActionType::Attack,"A"});check(r.phase_events.size()==1&&pve_hud(e.snapshot()).phase_id=="cycle-one","cyclic phase observed");
}
void defeat(){ScoreLedger ledger;auto d=fixture();d.roster[0].max_health=1;d.roster[1].speed=200;PveEncounter e(d,ledger);
 check(e.apply({"B",ActionType::Attack,"A"}).action.accepted(),"defeat action");const auto h=pve_hud(e.snapshot());check(h.end==PveEnd::TeamDefeated&&h.status_label=="Equipe derrotada"&&!h.objective_met&&!h.commands_open,"defeat status");
}
void invalid(){ScoreLedger ledger;PveEncounter e(fixture(),ledger);auto bad=e.snapshot();bad.boss_id="unknown";bool rejected=false;
 try{static_cast<void>(pve_hud(bad));}catch(const std::invalid_argument&){rejected=true;}check(rejected,"missing boss rejected");
 bad=e.snapshot();bad.finished=true;rejected=false;try{static_cast<void>(pve_hud(bad));}catch(const std::invalid_argument&){rejected=true;}check(rejected,"inconsistent closure rejected");
}
int main(){try{purity();clocks();ritual();score_window();survival();rejection();phases();cycles();defeat();invalid();std::cout<<"REG_HUD_TESTS_RESULT passed=10 total=10\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}