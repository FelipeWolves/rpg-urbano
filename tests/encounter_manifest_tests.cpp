#include "rpg/encounter_manifest.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
using namespace rpg;
void check(bool v){if(!v){throw std::runtime_error("Encounter manifest contract failed");}}
template<class F>void rejected(F f){bool caught=false;try{f();}catch(const ManifestError&){caught=true;}check(caught);}
int main(int argc,char** argv){try{
 for(auto profile:{CombatProfile::Team,CombatProfile::Classic}){auto m=reference_manifest(profile);check(decode_manifest(encode_manifest(m))==m);auto setup=compose_manifest(m);auto expected=profile==CombatProfile::Team?team_reference_setup():classic_reference_setup();check(setup==expected);}
 auto m=reference_manifest(CombatProfile::Team);auto invalid=m;invalid.schema=2;rejected([&]{static_cast<void>(encode_manifest(invalid));});invalid=m;invalid.roster[0].max_health++;rejected([&]{validate_manifest(invalid);});
 invalid=m;invalid.roster[0].id="unknown";rejected([&]{validate_manifest(invalid);});invalid=m;invalid.roster.push_back(invalid.roster[0]);rejected([&]{validate_manifest(invalid);});
 invalid=m;invalid.skills[0].first="missing";rejected([&]{validate_manifest(invalid);});invalid=m;invalid.bindings[0].slot=2;rejected([&]{validate_manifest(invalid);});
 auto bytes=encode_manifest(m);rejected([&]{static_cast<void>(decode_manifest(bytes+"extra"));});rejected([&]{static_cast<void>(decode_manifest(bytes.substr(0,30)));});rejected([&]{static_cast<void>(decode_manifest(std::string(65537,'x')));});
 auto overflow=bytes;auto pos=overflow.find("roster 6");overflow.replace(pos,8,"roster 999999999999999999");rejected([&]{static_cast<void>(decode_manifest(overflow));});
 if(argc!=2){throw std::runtime_error("Fixture directory required");}for(const auto name:{"team","classic"}){std::ifstream file(std::string(argv[1])+"/"+name+"-reference-v1.rpgmanifest",std::ios::binary);if(!file){throw std::runtime_error("Fixture missing");}const std::string text((std::istreambuf_iterator<char>(file)),{});auto decoded=decode_manifest(text);check(compose_manifest(decoded)==(decoded.profile==CombatProfile::Team?team_reference_setup():classic_reference_setup()));}
 std::cout<<"AUT14_TESTS_RESULT passed=7 total=7\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
