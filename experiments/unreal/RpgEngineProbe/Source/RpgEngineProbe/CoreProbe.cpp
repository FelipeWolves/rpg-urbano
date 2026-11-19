#include "CoreProbe.h"
#include "rpg/team_selection.hpp"
#include "rpg/local_session.hpp"
#include "rpg/save_game.hpp"
namespace rpg_probe {
Result Run() noexcept {
    Result result;
    auto require=[&](bool ok){++result.checkpoints;if(!ok){++result.failures;}};
    try{
        rpg::EncounterSetup setup;
        setup.roster={{"A1",rpg::Team::A,100,10,200},{"A2",rpg::Team::A,100,5,150},{"B1",rpg::Team::B,10,5,100}};
        setup.rules.skills={{"a1","A1",0,rpg::TargetRule::Enemy,{{rpg::EffectKind::Damage}}},
            {"a2","A2",0,rpg::TargetRule::Enemy,{{rpg::EffectKind::Damage}}},
            {"b1","B1",0,rpg::TargetRule::Enemy,{{rpg::EffectKind::Damage}}},
            {"emergency","A1",3,rpg::TargetRule::Self,{{rpg::EffectKind::Shield,10,2}}}};
        setup.loadouts={{"A1",{{1,"Basic","a1",{}},{3,"Emergency","emergency",{}}}},
            {"A2",{{1,"Basic","a2",{}}}},{"B1",{{1,"Basic","b1",{}}}}};
        setup.loadouts[0].slots[0].passive=rpg::PassiveDefinition{"opening"};
        setup.loadouts[0].slots[1].requirement=rpg::HealthRequirement{50};
        rpg::TeamSelection selection(setup);
        require(selection.select({"A1"})==rpg::SelectionError::None);
        const auto selected=selection.snapshot();
        require(selection.select({"A1","A1"})==rpg::SelectionError::DuplicateMember && selection.snapshot()==selected);
        const auto encounter=selection.compose();
        require(encounter.roster.size()==2 && encounter.rules.skills.size()==3 && encounter.loadouts.size()==2);
        rpg::LocalSession session(encounter.roster,encounter.rules,rpg::CombatProfile::Team,encounter.loadouts);
        require(session.battle().passive_activations().size()==1 && session.battle().battle().find("A1")->statuses[0].magnitude==10);
        const auto before=session.battle().snapshot();
        require(session.apply_slot(3,"A1").error==rpg::ActionError::SkillConditionNotMet && session.battle().snapshot()==before);
        require(session.apply_slot(1,"missing").error==rpg::ActionError::UnknownTarget && session.battle().snapshot()==before);
        const auto automated=session.battle().choose_automatic_action();
        require(automated && automated->skill=="a1" && session.battle().snapshot()==before);
        const auto applied=session.apply_slot(1,"B1");
        require(applied.accepted() && session.battle().battle().find("B1")->health==0 &&
            session.battle().battle().winner()==rpg::Team::A && session.battle().battle().accepted_actions()==1);
        const auto credited=session.progress();
        require(credited.credits==10 && credited.completions.size()==1 && !session.settle() &&
            !session.apply_slot(1,"B1").accepted() && session.progress()==credited);
        const rpg::SavedGame data{1,rpg::CombatProfile::Team,{"A1"},credited};
        const auto loaded=rpg::decode_save(rpg::encode_save(data));require(loaded==data);
        rpg::LocalSession resumed(encounter.roster,encounter.rules,rpg::CombatProfile::Team,encounter.loadouts);
        resumed.restore_progress(loaded.progress);
        require(resumed.progress().attempt==2 && resumed.progress().credits==10 && !resumed.settle() &&
            resumed.battle().battle().accepted_actions()==0);
        result.accepted=static_cast<int>(session.battle().battle().accepted_actions());
        result.credits=static_cast<int>(session.progress().credits);
    }catch(...){++result.failures;}
    result.passed=result.failures==0 && result.checkpoints==11;
    return result;
}
}
