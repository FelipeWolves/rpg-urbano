#include "rpg/visual_session.hpp"
#include "rpg/battle_presentation.hpp"
#include "rpg/startup_presentation.hpp"
#include <iostream>
#include <stdexcept>
using namespace rpg;
using namespace rpg::presentation;
void check(bool value){if(!value){throw std::runtime_error("Visual save contract failed");}}
template<class F> void rejected(F f){bool failed=false;try{f();}catch(const std::exception&){failed=true;}check(failed);}
int main(){try{
    ScreenFlow flow;check(flow.open_team() && flow.in_hub() && !flow.accepts_actions());check(!flow.enter_arena() && flow.back(false,false)==BackEffect::Transition && flow.screen()==UiScreen::Hub);
    EncounterSetup catalog;catalog.rules.action_budget=ActionBudget{10,LimitOutcome::Draw};
    for(int i=1;i<=6;++i){catalog.roster.push_back({"A"+std::to_string(i),Team::A,100,10,100-i});}
    catalog.roster.push_back({"B1",Team::B,100,10,90});
    for(const auto& unit:catalog.roster){const auto skill=unit.id+"-basic";catalog.rules.skills.push_back({skill,unit.id,0,TargetRule::Enemy,{{EffectKind::Damage}}});catalog.loadouts.push_back({unit.id,{{1,"Basic",skill,{}}}});}
    VisualSession five(catalog);check(five.team().selected.size()==5 && five.state().combat.battle.fighters.size()==6);
    check(five.select_team({"A1","A2","A3","A4","A5","A6"})==SelectionError::TooMany);
    check(five.select_team({"A6","A5","A4","A3","A2"})==SelectionError::None);
    VisualSession fiveLoaded(catalog);fiveLoaded.import_save(five.export_save());check(fiveLoaded.team().selected==five.team().selected);
    VisualSession classic(classic_reference_setup(),CombatProfile::Classic);VisualSession classicLoaded(classic_reference_setup(),CombatProfile::Classic);classicLoaded.import_save(classic.export_save());check(classicLoaded.state().combat.profile==CombatProfile::Classic);
    VisualSession session;const auto initial=session.state();
    check(session.select_team({})==SelectionError::Empty);
    check(session.select_team({"missing"})==SelectionError::UnknownMember);
    check(session.select_team({"A1","A1"})==SelectionError::DuplicateMember);
    check(session.state().revision==initial.revision && session.state().combat==initial.combat);
    check(session.select_team({"A2"})==SelectionError::None);
    check(session.team().selected==std::vector<std::string>{"A2"});
    check(session.state().combat.battle.fighters.size()==4 && session.state().progress.attempt==2);
    check(session.submit(initial.revision,1,"B1").error==VisualError::StaleCommand);
    const auto bytes=session.export_save();VisualSession restored;restored.import_save(bytes);
    check(restored.team().selected==session.team().selected && restored.state().progress.attempt==3);
    check(restored.state().combat.battle.accepted_actions==0 && restored.state().progress.credits==0);
    const auto unchanged=restored.state();
    rejected([&]{restored.import_save("corrupt");});
    auto bad=decode_save(bytes);bad.selected={"unknown"};rejected([&]{restored.import_save(encode_save(bad));});
    bad=decode_save(bytes);bad.profile=CombatProfile::Classic;rejected([&]{restored.import_save(encode_save(bad));});
    check(restored.state().revision==unchanged.revision && restored.state().combat==unchanged.combat && restored.state().progress==unchanged.progress);
    auto active=restored.state();check(active.automatic?restored.step_automatic().accepted():restored.submit(active.revision,1,"B1").accepted());
    const auto running=restored.state();rejected([&]{restored.import_save(bytes);});rejected([&]{static_cast<void>(restored.export_save());});rejected([&]{static_cast<void>(restored.select_team({"A1"}));});
    check(restored.state().combat==running.combat && restored.state().revision==running.revision);
    VisualSession battle;
    for(int i=0;i<1000 && battle.state().combat.battle.phase==Phase::Running;++i){const auto s=battle.state();if(s.automatic){check(battle.step_automatic().accepted());}else{const auto targets=target_candidates(s,1);check(!targets.empty());check(battle.submit(s.revision,1,targets.front()).accepted());}}
    check(battle.state().combat.battle.phase==Phase::Finished);
    const auto completed=battle.state().progress;VisualSession loaded;loaded.import_save(battle.export_save());
    check(loaded.state().progress.credits==completed.credits && loaded.state().progress.completions==completed.completions && loaded.state().progress.attempt==completed.attempt+1);
    const auto revision=loaded.state().revision;check(loaded.select_team({"A3","A1"})==SelectionError::None);
    check(loaded.team().selected==std::vector<std::string>({"A3","A1"}) && loaded.state().progress.credits==completed.credits && loaded.state().progress.completions==completed.completions && loaded.state().revision>revision);
    std::cout<<"VISUAL_SAVE_TESTS_RESULT passed=7 total=7\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
