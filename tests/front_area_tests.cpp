#include "rpg/shared_boss.hpp"
#include <iostream>
using namespace rpg;
void check(bool x,const char* m){if(!x){throw std::runtime_error(m);}}
SharedBossDefinition config(){
 SharedBossDefinition d;d.id="fronts";d.config_version=2;d.boss={"B",Team::B,1000,10,200};d.boss.stats.block=0;d.rules.combat_model=resolution::CombatModel::FunnelV1;d.rules.timing=TimingProfile::ActionBar;
 for(int i=1;i<=3;++i){Definition a{"A"+std::to_string(i),Team::A,100,10,50};a.stats.block=0;d.squads.push_back({"front"+std::to_string(i),{{a.id,a}}});}
 d.rules.skills={{"local","B",0,TargetRule::EnemyTeam,{{EffectKind::Damage}}},{"global","B",0,TargetRule::EnemyTeam,{{EffectKind::Damage}}},{"area","A1",0,TargetRule::EnemyTeam,{{EffectKind::Damage}}}};
 d.skill_scopes={{"local",SharedSkillTarget::RotatingFront},{"global",SharedSkillTarget::AllFronts},{"area",SharedSkillTarget::Boss}};
 return d;
}
int main(){try{
 SharedBoss local(config());auto before=local.snapshot();check(local.apply({"B",ActionType::Skill,"A3","local"}).error==SharedBossError::WrongFront&&local.snapshot()==before,"wrong front atomic");auto r=local.apply({"B",ActionType::Skill,"A1","local"});check(r.accepted()&&local.battle().find("A1")->health==90&&local.battle().find("A2")->health==100&&local.battle().find("A3")->health==100,"boss area local");
 SharedBoss global(config());r=global.apply({"B",ActionType::Skill,"A2","global"});check(r.accepted()&&global.battle().find("A1")->health==90&&global.battle().find("A2")->health==90&&global.battle().find("A3")->health==90,"global area all fronts");
 auto player=config();player.squads[0].members[0].fighter.speed=300;Definition x{"X",Team::B,100,1,10},y{"Y",Team::B,100,1,10};x.stats.block=0;y.stats.block=0;player.front_enemies={{"front1",x},{"front2",y}};SharedBoss area(player);r=area.apply({"A1",ActionType::Skill,"B","area"});check(r.accepted()&&area.battle().find("B")->health==990&&area.battle().find("X")->health==90&&area.battle().find("Y")->health==100,"player area only boss and own adds");auto bad=area.apply({"A1",ActionType::Attack,"Y"});check(bad.error==SharedBossError::WrongFront,"cannot attack other-front add");
 player.front_enemies[0].fighter.speed=350;SharedBoss auxiliary(player);auto action=auxiliary.choose_basic_action();check(action&&action->actor=="X"&&action->target=="A1"&&auxiliary.apply(*action).accepted(),"auxiliary own front auto target");
 auto kill=config();kill.squads[0].members[0].fighter.speed=300;kill.squads[0].members[0].fighter.attack=2000;kill.front_enemies={{"front1",x}};SharedBoss required(kill);r=required.apply({"A1",ActionType::Attack,"B"});check(r.accepted()&&r.stage_finished&&required.battle().find("X")->health==100,"main boss required with independent adds");
 auto passive=config();passive.squads[1].members[0].fighter.vigil=VigilDefinition{"watch",5,4};SkillDefinition basic{"watch","A2",0,TargetRule::Enemy,{{EffectKind::Damage}}};basic.basic=true;passive.rules.skills.push_back(basic);passive.skill_scopes.push_back({"watch",SharedSkillTarget::Boss});SharedBoss localPassive(passive);check(localPassive.apply({"B",ActionType::Skill,"A1","local"}).accepted()&&localPassive.battle().find("A2")->vigil_charges==4,"passive does not count other fronts");
 SharedBoss globalPassive(passive);check(globalPassive.apply({"B",ActionType::Skill,"A2","global"}).accepted()&&globalPassive.battle().find("A2")->vigil_charges==0&&!has_status(*globalPassive.battle().find("A1"),EffectKind::StandardShield)&&has_status(*globalPassive.battle().find("A2"),EffectKind::StandardShield),"global hit triggers own front passive and own front shield");
 std::cout<<"REG_FRONT_TESTS_RESULT passed=8 total=8\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
