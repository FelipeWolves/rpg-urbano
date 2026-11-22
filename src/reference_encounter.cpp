#include "rpg/reference_encounter.hpp"
#include <stdexcept>
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

namespace rpg::presentation {
std::vector<InspectionFrame> arena_inspection_frames(){
    std::vector<InspectionFrame> frames;
    for(const auto pair:std::vector<std::pair<int,int>>{{1,1},{2,1},{3,3},{5,5},{1,5},{5,1}}){
        std::vector<Definition> roster;
        for(const auto side:{Team::A,Team::B}){
            const int count=side==Team::A?pair.first:pair.second;
            for(int i=0;i<count;++i){roster.push_back({std::string(side==Team::A?"A":"B")+std::to_string(i+1),side,100,8,(side==Team::A?300:200)-i});}
        }
        Battle battle(roster);
        frames.push_back({"Formation"+std::to_string(pair.first)+"v"+std::to_string(pair.second),"Formacao sem simulacao automatica",battle.snapshot(),"A1",{"Somente inspecao: nao concede recompensa.","Usa o posicionamento e as barras reais da arena."}});
    }
    BattleRules rules;
    rules.skills={{"inspect-shield","A1",0,TargetRule::Ally,{{EffectKind::StandardShield}}}};
    const auto makeBattle=[&](int attack){return Battle({{"A1",Team::A,1000,1,300},{"A2",Team::A,100,1,100},{"B1",Team::B,1000,attack,200}},rules,{{"A1",1000},{"A2",50},{"B1",1000}});};
    Battle before=makeBattle(8);
    const auto capture=[&](const char* id,const Battle& b,std::vector<std::string> notes){frames.push_back({id,"Escudo padrao em A2",b.snapshot(),"A2",std::move(notes)});};
    const auto apply=[](Battle& b,Action action){if(!b.apply(action).accepted()){throw std::logic_error("Inspection fixture action rejected");}};
    capture("ShieldBefore",before,{"A2: 50/100 PV; sem escudo."});
    Battle applied=before;apply(applied,{"A1",ActionType::Skill,"A2","inspect-shield"});
    capture("ShieldApplied",applied,{"A1: 1000 PV maximos -> escudo 10 por 3 turnos naturais.","A2: 50/100 PV; faixa = 10% da barra inteira (20% da vida preenchida)."});
    Battle consumed=applied;apply(consumed,{"B1",ActionType::Attack,"A2",{}});
    capture("ShieldConsumed",consumed,{"Dano 8: escudo 10 -> 2; A2 permanece com 50 PV."});
    Battle reapplied=consumed;apply(reapplied,{"A2",ActionType::Attack,"B1",{}});apply(reapplied,{"A1",ActionType::Skill,"A2","inspect-shield"});
    capture("ShieldReapplied",reapplied,{"Nova aplicacao restaura forca 10 e duracao 3; nao soma 2+10."});
    Battle exhausted=makeBattle(18);apply(exhausted,{"A1",ActionType::Skill,"A2","inspect-shield"});apply(exhausted,{"B1",ActionType::Attack,"A2",{}});
    capture("ShieldExhausted",exhausted,{"Dano 18: absorve 10 e passa 8 ao PV; A2 fica com 42/100, sem icone de escudo."});
    Battle expired=applied;
    for(int i=0;i<16;++i){
        const auto* fighter=expired.find("A2");bool shield=false;
        for(const auto& status:fighter->statuses){shield=shield || status.kind==EffectKind::StandardShield;}
        if(!shield){break;}
        const auto* actor=expired.current_actor();
        apply(expired,{actor->definition.id,ActionType::Attack,actor->definition.team==Team::B?"A1":"B1",{}});
    }
    capture("ShieldExpired",expired,{"Depois de 3 turnos naturais de A2: sem escudo, 50/100 PV.","Ataques basicos aos demais alvos avancaram o relogio sem causar dano em A2."});
    Battle dead=makeBattle(100);apply(dead,{"A1",ActionType::Skill,"A2","inspect-shield"});apply(dead,{"B1",ActionType::Attack,"A2",{}});
    capture("ShieldDeath",dead,{"Dano letal remove o escudo; A2 fica com 0 PV e seu modelo nao deve aparecer."});
    return frames;
}
}
