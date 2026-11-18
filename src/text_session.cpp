#include "rpg/text_session.hpp"
#include <iostream>
#include <sstream>

namespace rpg::presentation {
namespace {
void help(std::ostream& out){
    out<<"Comandos: atacar ALVO | guardar | habilidade ID ALVO | passar\n"
       <<"          estado | ajuda | reiniciar | sair\n"
       <<"Voce controla o time A. O time B ataca automaticamente.\n";
}
const char* error_message(ActionError error){
    switch(error){
    case ActionError::BattleFinished:return "A batalha terminou. Use reiniciar ou sair.";
    case ActionError::UnknownTarget:return "Alvo desconhecido. Consulte estado.";
    case ActionError::TargetDefeated:return "Esse alvo esta derrotado.";
    case ActionError::FriendlyTarget:return "O ataque exige um inimigo.";
    case ActionError::GuardDisabled:return "Guarda desabilitada neste perfil.";
    case ActionError::UnknownSkill:return "Habilidade desconhecida para o personagem atual.";
    case ActionError::SkillOnCooldown:return "A habilidade ainda esta em recarga.";
    case ActionError::InvalidSkillTarget:return "Alvo invalido para a habilidade.";
    case ActionError::ActorBlocked:return "Personagem bloqueado: use passar.";
    case ActionError::NotBlocked:return "Passar so e permitido quando o personagem esta bloqueado.";
    default:return "Acao recusada pelo motor. Consulte estado.";
    }
}
void state(const Battle& battle,std::ostream& out){
    out<<"\n--- Estado | acoes "<<battle.accepted_actions()<<" ---\n";
    for(const auto& fighter:battle.fighters()){
        out<<fighter.definition.id<<" ["<<(fighter.definition.team==Team::A?'A':'B')<<"] vida "
           <<fighter.health<<'/'<<fighter.definition.max_health<<" SPD "<<fighter.effective_speed;
        if(fighter.health==0){out<<" DERROTADO";}else if(fighter.guarding){out<<" GUARDA";}
        out<<'\n';
    }
    if(battle.phase()==Phase::Finished){
        if(battle.end_reason()==EndReason::TechnicalLimit){out<<"Batalha interrompida por limite tecnico, sem vencedor.\n";}
        else{out<<"Resultado: ";if(battle.winner()){out<<(*battle.winner()==Team::A?"vitoria do time A":"vitoria do time B");}
            else{out<<"empate";}out<<(battle.end_reason()==EndReason::ActionLimit?" (limite de acoes)":" (eliminacao)")<<".\n";}
        out<<"Use reiniciar ou sair.\n";return;
    }
    const auto* actor=battle.current_actor();
    if(actor){out<<"Vez de "<<actor->definition.id;if(battle.current_actor_blocked()){out<<" [bloqueado: passar]";}out<<'\n';
        for(const auto& skill:battle.snapshot().rules.skills){if(skill.owner==actor->definition.id){
            out<<"Habilidade "<<skill.id<<" | alvo "<<(skill.target==TargetRule::Self?"proprio":skill.target==TargetRule::Ally?"aliado":"inimigo");
            for(const auto& cd:actor->cooldowns){if(cd.skill==skill.id){out<<" | recarga "<<cd.remaining;}}
            out<<'\n';}}
    }
    if(const auto remaining=battle.remaining_actions()){out<<"Acoes restantes da batalha: "<<*remaining<<'\n';}
}
void events(const ActionResult& result,std::ostream& out){
    for(const auto& e:result.events){switch(e.type){
    case EventType::Attack:case EventType::Reaction:case EventType::ReflectedDamage:case EventType::DeathExplosion:
        out<<e.actor<<" -> "<<e.target<<" dano "<<e.amount;
        if(e.type==EventType::Reaction){out<<" [reacao]";}else if(e.type==EventType::ReflectedDamage){out<<" [reflexao]";}
        else if(e.type==EventType::DeathExplosion){out<<" [explosao]";}out<<'\n';break;
    case EventType::GuardSet:out<<e.actor<<" usou guarda.\n";break;
    case EventType::SkillUsed:out<<e.actor<<" usou habilidade.\n";break;
    case EventType::FighterDefeated:out<<e.actor<<" foi derrotado.\n";break;
    case EventType::FighterRevived:out<<e.actor<<" reviveu.\n";break;
    case EventType::TurnSkipped:out<<e.actor<<" passou por bloqueio.\n";break;
    default:break;
    }}
}
}
BattleRules text_reference_rules(){
    BattleRules rules;rules.timing=TimingProfile::ActionBar;rules.action_budget=ActionBudget{200,LimitOutcome::Draw};return rules;
}
TextReport run_text_session(std::istream& input,std::ostream& output,std::vector<Definition> roster,BattleRules rules){
    Battle battle(roster,rules);std::uint64_t players=0,automatic=0,rejected=0,restarts=0;
    TextExit exit=TextExit::InputClosed;
    auto advance_enemy=[&](){
        std::uint32_t burst=0;
        while(battle.phase()==Phase::Running && battle.current_actor()->definition.team==Team::B){
            if(burst++==256){output<<"Apresentacao interrompida por excesso de acoes automaticas; nenhum resultado inventado.\n";return false;}
            const auto action=choose_basic_attack(battle);
            if(!action){output<<"Apresentacao interrompida: nenhuma acao automatica disponivel.\n";return false;}
            const auto result=battle.apply(*action);
            if(!result.accepted()){output<<"Apresentacao interrompida: acao automatica recusada.\n";return false;}
            ++automatic;events(result,output);
        }
        return true;
    };
    output<<"RPG Urbano - laboratorio textual\n";help(output);
    if(!advance_enemy()){return {battle.snapshot(),TextExit::AutomaticSafetyStop,players,automatic,rejected,restarts};}
    state(battle,output);
    std::string line;
    for(;;){
        output<<"> "<<std::flush;
        if(!std::getline(input,line)){output<<"Entrada encerrada.\n";break;}
        std::istringstream parser(line);std::vector<std::string> tokens;std::string word;
        while(parser>>word){tokens.push_back(word);}
        if(tokens.empty()){continue;}
        const auto& command=tokens[0];
        if(tokens.size()==1 && command=="sair"){exit=TextExit::Quit;output<<"Sessao encerrada.\n";break;}
        if(tokens.size()==1 && command=="ajuda"){help(output);continue;}
        if(tokens.size()==1 && command=="estado"){state(battle,output);continue;}
        if(tokens.size()==1 && command=="reiniciar"){
            battle=Battle(roster,rules);++restarts;output<<"Batalha reiniciada.\n";
            if(!advance_enemy()){exit=TextExit::AutomaticSafetyStop;break;}state(battle,output);continue;
        }
        Action action;const auto* actor=battle.current_actor();action.actor=actor?actor->definition.id:std::string{};
        if(command=="atacar" && tokens.size()==2){action.type=ActionType::Attack;action.target=tokens[1];}
        else if(command=="guardar" && tokens.size()==1){action.type=ActionType::Guard;}
        else if(command=="passar" && tokens.size()==1){action.type=ActionType::Skip;}
        else if(command=="habilidade" && tokens.size()==3){action.type=ActionType::Skill;action.skill=tokens[1];action.target=tokens[2];}
        else{++rejected;output<<"Comando invalido. Use ajuda.\n";continue;}
        const auto result=battle.apply(action);
        if(!result.accepted()){++rejected;output<<error_message(result.error)<<'\n';continue;}
        ++players;events(result,output);
        if(!advance_enemy()){exit=TextExit::AutomaticSafetyStop;break;}
        state(battle,output);
    }
    return {battle.snapshot(),exit,players,automatic,rejected,restarts};
}
} // namespace rpg::presentation
