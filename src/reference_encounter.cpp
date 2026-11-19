#include "rpg/reference_encounter.hpp"
namespace rpg {
BattleRules text_reference_rules(){
    BattleRules rules;rules.timing=TimingProfile::ActionBar;rules.action_budget=ActionBudget{200,LimitOutcome::Draw};return rules;
}
EncounterSetup team_reference_setup(){
    auto roster=reference_roster();auto rules=text_reference_rules();std::vector<CharacterLoadout> sets;
    for(const auto& unit:roster){
        const auto basic=unit.id+"-basica",special=unit.id+"-especial",third=unit.id+"-terceira";
        rules.skills.push_back({basic,unit.id,0,TargetRule::Enemy,{{EffectKind::Damage}}});
        CharacterLoadout set{unit.id,{{1,"Ataque basico",basic,{}}}};
        EffectDefinition double_hit{EffectKind::Damage};double_hit.hit_count=2;
        if(unit.id.back()=='2'){
            set.slots.push_back({2,"Preparacao",std::nullopt,"Escudo de abertura; uma vez por combate.",
                PassiveDefinition{unit.id+"-preparacao"}});
        }else{
            rules.skills.push_back({special,unit.id,2,TargetRule::Enemy,{double_hit}});
            set.slots.push_back({2,"Golpe duplo",special,unit.id.back()=='3'?"Componente hibrido reservado; sem gatilho passivo.":""});
            if(unit.id.back()=='3'){
                auto& hybrid=set.slots.back();hybrid.name="Golpe duplo preparado";
                hybrid.passive_note="Escudo de abertura; componente ativo usa recarga propria.";
                hybrid.passive=PassiveDefinition{unit.id+"-preparacao-hibrida"};
            }
        }
        rules.skills.push_back({third,unit.id,3,TargetRule::Self,{{EffectKind::StandardShield}}});
        AbilitySlot protection{3,"Protecao",third,{}};
        if(unit.id.back()=='3'){protection.name="Protecao de emergencia";protection.requirement=HealthRequirement{50};}
        set.slots.push_back(std::move(protection));sets.push_back(std::move(set));
    }
    return {std::move(roster),std::move(rules),std::move(sets)};
}
EncounterSetup classic_reference_setup(){return {reference_roster(),text_reference_rules(),{}};}
}
