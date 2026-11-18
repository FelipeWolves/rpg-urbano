#include "rpg/text_session.hpp"
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
          out.str().find("Perfil: EQUIPES")!=std::string::npos && out.str().find("componente passivo ainda nao implementado")!=std::string::npos,
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
}
int main(int argc,char** argv){
    const std::pair<const char*,void(*)()> tests[]={{"input_atomics",input_and_invalid_atomics},{"target_enemy",selected_target_and_enemy},
        {"guard_restart",guard_and_restart},{"victory_restart",victory_postfinish_restart},
        {"skill_skip_clock",skill_block_skip_and_clock},{"eof_timeout",eof_and_timeout},{"safety_technical",safety_and_technical_result},
        {"team_profile",team_profile_session},{"opening_passive_session",opening_passive_session},{"conditional_skill_session",conditional_skill_session},{"automatic_special_session",automatic_special_session}};
    int passed=0;for(const auto& [name,test]:tests){try{test();++passed;std::cout<<"PASS text "<<name<<'\n';}
        catch(const std::exception& e){std::cerr<<"FAIL text "<<name<<": "<<e.what()<<'\n';}}
    std::cout<<"TEXT_TESTS_RESULT passed="<<passed<<" total="<<std::size(tests)<<'\n';
    if(argc==2 && passed==static_cast<int>(std::size(tests))){std::ofstream out(argv[1]);out<<"test,status,scope\n";
        for(const auto& [name,test]:tests){static_cast<void>(test);out<<name<<",pass,text_session_player_A_basic_AI_B\n";}if(!out){return 2;}}
    return passed==static_cast<int>(std::size(tests))?0:1;
}
