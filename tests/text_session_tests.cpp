#include "rpg/text_session.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
using namespace rpg;using namespace rpg::presentation;
void check(bool ok,const char* message){if(!ok){throw std::runtime_error(message);}}
std::vector<Definition> duel(){return {{"A",Team::A,10,3,200},{"B",Team::B,10,2,100}};}
BattleRules rules(){BattleRules r;r.timing=TimingProfile::FixedRounds;return r;}
void input_and_invalid_atomics(){
    std::istringstream in("\n  \najuda\nestado\natacar B extra\nreiniciar extra\natacar missing\natacar A\npassar\nsair\n");std::ostringstream out;
    const auto roster=duel();const auto r=rules();const auto result=run_text_session(in,out,roster,r);
    check(result.final_state==Battle(roster,r).snapshot() && result.player_actions==0 && result.automatic_actions==0 &&
          result.rejected_commands==5 && result.exit==TextExit::Quit,"invalid/help/state consumed actions or wrong parsing");
    check(out.str().find("Alvo desconhecido")!=std::string::npos && out.str().find("exige um inimigo")!=std::string::npos,
          "unhelpful rejection output");
}
void selected_target_and_enemy(){
    auto roster=duel();roster.push_back({"B2",Team::B,20,1,50});std::istringstream in("atacar B2\nsair\n");std::ostringstream out;
    const auto result=run_text_session(in,out,roster,rules());Battle expected(roster,rules());
    check(expected.apply({"A",ActionType::Attack,"B2"}).accepted(),"expected action invalid");
    while(expected.phase()==Phase::Running && expected.current_actor()->definition.team==Team::B){
        const auto cmd=choose_basic_attack(expected);check(cmd && expected.apply(*cmd).accepted(),"expected AI invalid");}
    check(result.final_state==expected.snapshot() && result.player_actions==1 && result.automatic_actions==2 &&
          out.str().find("A -> B2 dano 3")!=std::string::npos,"target choice or enemy response wrong");
}
void guard_and_restart(){
    std::istringstream in("defesa\nreiniciar\nsair\n");std::ostringstream out;const auto roster=duel();const auto r=rules();
    const auto result=run_text_session(in,out,roster,r);
    check(result.player_actions==1 && result.automatic_actions==1 && result.restarts==1 &&
          result.final_state==Battle(roster,r).snapshot() && out.str().find("dano 1")!=std::string::npos,
          "guard not delegated or restart retained state");
}
void victory_postfinish_restart(){
    auto roster=duel();roster[1].max_health=3;std::istringstream in("atacar B\natacar B\nestado\nreiniciar\nsair\n");std::ostringstream out;
    const auto result=run_text_session(in,out,roster,rules());
    check(result.player_actions==1 && result.automatic_actions==0 && result.rejected_commands==1 && result.restarts==1 &&
          result.final_state==Battle(roster,rules()).snapshot() && out.str().find("vitoria do time A")!=std::string::npos,
          "victory allowed enemy/post-finish action or reset failed");
}
void skill_block_skip_and_clock(){
    auto r=rules();r.skills={{"block","A",2,TargetRule::Enemy,{{EffectKind::Block,0,2}}}};
    std::istringstream in("habilidade block B\nhabilidade block B\nsair\n");std::ostringstream out;
    const auto result=run_text_session(in,out,duel(),r);
    check(result.player_actions==1 && result.automatic_actions==1 && result.rejected_commands==1 &&
          out.str().find("passou por bloqueio")!=std::string::npos && out.str().find("em recarga")!=std::string::npos,
          "skill/cooldown or automatic blocked skip wrong");
    r=rules();r.skills={{"block","A",0,TargetRule::Self,{{EffectKind::Block,0,3}}}};
    std::istringstream blocked("habilidade block A\npassar\nsair\n");std::ostringstream skipped;
    const auto player=run_text_session(blocked,skipped,duel(),r);
    check(player.player_actions==2 && player.automatic_actions==2 && player.rejected_commands==0 &&
          skipped.str().find("A passou por bloqueio")!=std::string::npos,"player blocked Skip not handled");
}
void eof_and_timeout(){
    std::istringstream eof;std::ostringstream output;const auto empty=run_text_session(eof,output,duel(),rules());
    check(empty.exit==TextExit::InputClosed && empty.player_actions==0,"EOF looped or advanced player");
    auto r=rules();r.action_budget=ActionBudget{1,LimitOutcome::Draw};std::istringstream in("defesa\nsair\n");std::ostringstream out;
    const auto result=run_text_session(in,out,duel(),r);
    check(result.final_state.end_reason==EndReason::ActionLimit && !result.final_state.winner && result.automatic_actions==0 &&
          out.str().find("empate (limite de acoes)")!=std::string::npos,"timeout printed false winner or started AI");
    auto roster=duel();roster[0].max_health=1;std::istringstream loss("defesa\nsair\n");std::ostringstream lost;
    const auto defeat=run_text_session(loss,lost,roster,rules());
    check(defeat.final_state.winner==Team::B && defeat.automatic_actions==1 &&
          lost.str().find("vitoria do time B")!=std::string::npos,"defeat presented as player victory");
}
void safety_and_technical_result(){
    auto roster=duel();roster[0].max_health=10000;roster[0].speed=1;roster[1].speed=1000;auto r=rules();r.timing=TimingProfile::ActionBar;
    std::istringstream noinput;std::ostringstream output;const auto paused=run_text_session(noinput,output,roster,r);
    check(paused.exit==TextExit::AutomaticSafetyStop && paused.automatic_actions==256 && paused.final_state.phase==Phase::Running &&
          !paused.final_state.winner,"presentation safety fabricated battle outcome");
    r=rules();r.max_derived_actions=1;r.skills={{"extra","A",0,TargetRule::Self,{{EffectKind::ExtraTurn},{EffectKind::ExtraTurn}}}};
    std::istringstream technical("habilidade extra A\nsair\n");std::ostringstream out;
    const auto ended=run_text_session(technical,out,duel(),r);
    check(ended.final_state.end_reason==EndReason::TechnicalLimit && !ended.final_state.winner &&
          out.str().find("limite tecnico, sem vencedor")!=std::string::npos,"technical stop printed competitive winner");
}
void team_profile_session(){
    std::istringstream input("defesa\n2 B3\nreiniciar\natacar B3\nsair\n");std::ostringstream out;
    const auto r=run_team_reference_session(input,out);
    check(r.player_actions==2 && r.rejected_commands==1 && r.restarts==1 && !r.final_state.rules.guard_enabled &&
          out.str().find("Perfil: EQUIPES")!=std::string::npos && out.str().find("componente passivo automatico")!=std::string::npos,
          "team UI allowed defense, lost aliases/reset or claimed implemented passive");
    check(out.str().find("1 - Ataque basico | DISPONIVEL")!=std::string::npos,"basic slot not shown");
}
void opening_passive_session(){
    auto r=rules();r.skills={{"a","A",0,TargetRule::Enemy,{{EffectKind::Damage}}},
        {"b","B",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
    std::vector<CharacterLoadout> sets{{"A",{{1,"Basica","a",{}},
        {2,"Preparacao",std::nullopt,"Escudo de abertura",PassiveDefinition{"opening-a"}}}},
        {"B",{{1,"Basica","b",{}}}}};
    std::istringstream in("2 B\nestado\n1 B\nreiniciar\nsair\n");std::ostringstream out;
    const auto report=run_text_session(in,out,duel(),r,CombatProfile::Team,sets);
    const auto text=out.str();const auto first=text.find("ativou passiva opening-a");
    const auto second=text.find("ativou passiva opening-a",first+1);
    check(report.rejected_commands==1 && report.player_actions==1 && report.restarts==1 &&
          report.final_state.accepted_actions==0,"passive command or reset counted incorrectly");
    check(first!=std::string::npos && second!=std::string::npos &&
          text.find("ativou passiva opening-a",second+1)==std::string::npos,"opening log duplicated or lost reset");
    check(text.find("PASSIVA AUTOMATICA")!=std::string::npos && text.find("ESCUDO 10")!=std::string::npos,
          "functional passive or shield not visible");
}
void conditional_skill_session(){
    auto r=rules();r.skills={{"a","A",0,TargetRule::Enemy,{{EffectKind::Damage}}},
        {"shield","A",3,TargetRule::Self,{{EffectKind::Shield,10,2}}},
        {"b","B",0,TargetRule::Enemy,{{EffectKind::Damage}}}};
    std::vector<CharacterLoadout> sets{{"A",{{1,"Basica","a",{}},{3,"Emergencia","shield",{}}}},
        {"B",{{1,"Basica","b",{}}}}};sets[0].slots[1].requirement=HealthRequirement{50};
    auto roster=duel();roster[1].attack=5;
    std::istringstream in("3 A\nhabilidade shield A\n1 B\n3 A\nestado\nsair\n");std::ostringstream out;
    const auto report=run_text_session(in,out,roster,r,CombatProfile::Team,sets);
    check(report.rejected_commands==2 && report.player_actions==2 && report.automatic_actions==2,
          "condition rejection consumed action or valid shield failed");
    const auto text=out.str();check(text.find("BLOQUEADA: condicao nao satisfeita")!=std::string::npos &&
        text.find("vida <= 50% [nao atendido]")!=std::string::npos && text.find("vida <= 50% [atendido]")!=std::string::npos &&
        text.find("RECARGA: 2")!=std::string::npos && text.find("ESCUDO 5")!=std::string::npos,"condition/CD/output unclear");
}
void automatic_special_session(){
    auto roster=duel();roster[0].max_health=100;roster[1].max_health=100;auto r=rules();
    r.skills={{"a","A",0,TargetRule::Enemy,{{EffectKind::Damage}}},
        {"b","B",0,TargetRule::Enemy,{{EffectKind::Damage}}},
        {"b-special","B",3,TargetRule::Enemy,{{EffectKind::Damage}}}};
    std::vector<CharacterLoadout> sets{{"A",{{1,"Basica","a",{}}}},
        {"B",{{1,"Basica","b",{}},{3,"Especial","b-special",{}}}}};
    std::istringstream in("1 B\n1 B\nsair\n");std::ostringstream out;
    const auto report=run_text_session(in,out,roster,r,CombatProfile::Team,sets);
    check(report.player_actions==2 && report.automatic_actions==2 && report.rejected_commands==0 &&
          out.str().find("Auto B: b-special -> A")!=std::string::npos &&
          out.str().find("Auto B: b -> A")!=std::string::npos,"text AI special/fallback not integrated");
    ProfileBattle expected(roster,r,CombatProfile::Team,sets);
    for(int i=0;i<2;++i){check(expected.apply_slot(1,"B").accepted() && expected.apply(*expected.choose_automatic_action()).accepted(),"expected AI failed");}
    check(report.final_state==expected.battle().snapshot(),"text policy changed rules or timing");
}
void integrated_reference_kits(){
    const auto d=team_reference_setup();ProfileBattle s(d.roster,d.rules,CombatProfile::Team,d.loadouts);
    check(d.roster.size()==6 && s.passive_activations().size()==4 && s.battle().accepted_actions()==0,"reference kit opening mismatch");
    check(s.battle().find("A1")->statuses.empty() && s.battle().find("A2")->statuses[0].magnitude==10 &&
          s.battle().find("A3")->statuses[0].magnitude==10,"active/passive/hybrid kit not functional");
    const auto active=s.abilities("A1"),passive=s.abilities("A2"),hybrid=s.abilities("A3");
    check(active[1].slot.active_skill && !active[1].passive_implemented && !passive[1].slot.active_skill &&
          passive[1].passive_implemented && hybrid[1].slot.active_skill && hybrid[1].passive_implemented &&
          hybrid[2].availability==AbilityAvailability::ConditionBlocked,"kit categories lost or condition missing");
    const auto before=s.snapshot();check(s.apply_slot(3,"A3").error==ActionError::SkillConditionNotMet && s.snapshot()==before,
                                        "initial emergency bypassed condition");
    check(s.apply_slot(2,"B3").accepted() && s.battle().find("B3")->health==40 &&
          s.battle().find("A3")->statuses[0].magnitude==10,"hybrid active failed or consumed own passive");
    const auto response=s.choose_automatic_action();check(response && response->skill=="B3-terceira" &&
          s.apply(*response).accepted() && s.battle().find("B3")->statuses[0].magnitude==10,"hybrid/threshold/enemy automation not integrated");
}
struct ReferenceScript {
    std::string input;
    Snapshot final_state;
    std::uint64_t players=0,enemies=0,rejected=0;
};
ReferenceScript complete_reference_script(){
    const auto d=team_reference_setup();ProfileBattle battle(d.roster,d.rules,CombatProfile::Team,d.loadouts);
    ReferenceScript script;script.input="ajuda\nestado\n";
    while(battle.battle().phase()==Phase::Running){
        const auto action=battle.choose_automatic_action();check(action.has_value(),"reference automatic choice missing");
        if(battle.battle().current_actor()->definition.team==Team::A){
            script.input+="defesa\n";++script.rejected;
            const auto* actor=battle.battle().current_actor();
            if(actor->definition.id=="A2"){script.input+="2 B1\n";++script.rejected;}
            if(action->type==ActionType::Skip){script.input+="passar\n";}
            else{const auto views=battle.abilities(action->actor);std::uint32_t position=0;
                for(const auto& view:views){if(view.slot.active_skill==action->skill){position=view.slot.position;}}
                check(position>0,"reference action not in menu");script.input+=std::to_string(position)+" "+action->target+"\n";}
            ++script.players;
        }else{++script.enemies;}
        check(battle.apply(*action).accepted(),"reference full-battle action invalid");
    }
    script.input+="1 B1\n";++script.rejected;script.final_state=battle.battle().snapshot();return script;
}
void integrated_full_text_battle(){
    const auto script=complete_reference_script();std::istringstream in(script.input+"sair\n");std::ostringstream out;
    const auto report=run_team_reference_session(in,out);
    check(report.exit==TextExit::Quit && report.final_state==script.final_state && report.player_actions==script.players &&
          report.automatic_actions==script.enemies && report.rejected_commands==script.rejected,"full textual battle diverged from profile");
    check(report.final_state.phase==Phase::Finished && report.final_state.accepted_actions<=200 &&
          out.str().find("Resultado:")!=std::string::npos && out.str().find("componente passivo automatico")!=std::string::npos,
          "full battle missing end or real hybrid display");
}
void integrated_full_reset(){
    const auto script=complete_reference_script();std::istringstream in(script.input+"estado\nreiniciar\nsair\n");std::ostringstream out;
    const auto report=run_team_reference_session(in,out);const auto d=team_reference_setup();
    ProfileBattle fresh(d.roster,d.rules,CombatProfile::Team,d.loadouts);
    check(report.restarts==1 && report.final_state==fresh.battle().snapshot() && report.rejected_commands==script.rejected,
          "full reset retained effects/cooldowns/health or policy changed");
    const auto text=out.str();const std::string marker="A3 ativou passiva A3-preparacao-hibrida";
    const auto first=text.find(marker),second=text.find(marker,first+1);
    check(first!=std::string::npos && second!=std::string::npos && text.find(marker,second+1)==std::string::npos,
          "hybrid opening duplicated across actions/reset");
}
void integrated_reference_matrix(){
    for(int i=0;i<8;++i){
        auto d=team_reference_setup();d.rules.action_budget=ActionBudget{12,LimitOutcome::Draw};
        d.rules.timing=i%2?TimingProfile::ActionBar:TimingProfile::FixedRounds;
        for(auto& set:d.loadouts){if(i%4==0){set.automatic_priority.clear();}else if(i%4==1){set.automatic_priority={2,3};}}
        ProfileBattle a(d.roster,d.rules,CombatProfile::Team,d.loadouts),b(d.roster,d.rules,CombatProfile::Team,d.loadouts);
        const auto initial=a.snapshot();int finishes=0;
        while(a.battle().phase()==Phase::Running){const auto choice=a.choose_automatic_action();check(choice.has_value(),"matrix choice missing");
            const auto result=a.apply(*choice);check(result.accepted() && result==b.apply(*b.choose_automatic_action()) &&
                a.snapshot()==b.snapshot(),"integrated kit replay mismatch");
            finishes+=static_cast<int>(std::ranges::count(result.events,EventType::BattleFinished,&Event::type));}
        check(finishes==1 && a.battle().accepted_actions()<=12 && a.passive_activations().size()==4,"matrix budget/end/passive count failed");
        a.reset();check(a.snapshot()==initial,"matrix reset mismatch");
    }
}
void selected_team_session(){
    std::istringstream in("1 B3\nreiniciar\nsair\n");std::ostringstream out;
    const auto report=run_selected_team_reference_session(in,out,{"A3"});
    check(report.player_actions==1 && report.restarts==1 && report.final_state.fighters.size()==4 &&
        !std::ranges::any_of(report.final_state.fighters,[](const Fighter& f){return f.definition.id=="A1" || f.definition.id=="A2";}),
        "selection not reflected in session/reset");
    const auto d=team_reference_setup();TeamSelection selection(d);check(selection.select({"A3"})==SelectionError::None,"expected selection failed");
    const auto setup=selection.compose();ProfileBattle expected(setup.roster,setup.rules,CombatProfile::Team,setup.loadouts);
    check(report.final_state==expected.battle().snapshot(),"selected reset differs from fresh encounter");
    std::istringstream bad("sair\n");std::ostringstream errors;bool caught=false;
    try{const auto ignored=run_selected_team_reference_session(bad,errors,{"B1"});static_cast<void>(ignored);}
    catch(const std::invalid_argument&){caught=true;}
    check(caught && errors.str().empty(),"invalid selection started battle");
}
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"input_atomics",input_and_invalid_atomics},{"target_enemy",selected_target_and_enemy},
        {"guard_restart",guard_and_restart},{"victory_restart",victory_postfinish_restart},
        {"skill_skip_clock",skill_block_skip_and_clock},{"eof_timeout",eof_and_timeout},{"safety_technical",safety_and_technical_result},
        {"team_profile",team_profile_session},{"opening_passive_session",opening_passive_session},{"conditional_skill_session",conditional_skill_session},{"automatic_special_session",automatic_special_session},
        {"integrated_reference_kits",integrated_reference_kits},{"integrated_full_text_battle",integrated_full_text_battle},
        {"integrated_full_reset",integrated_full_reset},{"integrated_reference_matrix",integrated_reference_matrix},{"selected_team_session",selected_team_session}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS text "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL text "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"TEXT_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,text_session_player_A_basic_AI_B\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
