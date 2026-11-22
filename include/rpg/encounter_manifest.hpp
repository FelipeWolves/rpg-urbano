#pragma once
#include "rpg/reference_encounter.hpp"
#include <charconv>
#include <locale>
#include <limits>
#include <sstream>
#include <algorithm>
namespace rpg {
struct ManifestBinding {std::string owner;std::uint32_t slot;std::string active,passive;bool operator==(const ManifestBinding&) const=default;};
struct EncounterManifest {
    std::uint32_t schema=1;
    std::string scenario;
    CombatProfile profile=CombatProfile::Team;
    std::vector<Definition> roster;
    std::vector<std::pair<std::string,std::string>> skills;
    std::vector<ManifestBinding> bindings;
    bool operator==(const EncounterManifest&) const=default;
};
class ManifestError:public std::runtime_error {public:using std::runtime_error::runtime_error;};
inline EncounterManifest reference_manifest(CombatProfile profile){
    if(profile!=CombatProfile::Team && profile!=CombatProfile::Classic){throw ManifestError("Perfil desconhecido.");}
    const auto setup=profile==CombatProfile::Team?team_reference_setup():classic_reference_setup();
    EncounterManifest result;result.profile=profile;result.scenario=profile==CombatProfile::Team?"team-reference-v1":"classic-reference-v1";result.roster=setup.roster;
    for(const auto& skill:setup.rules.skills){result.skills.emplace_back(skill.owner,skill.id);}
    for(const auto& loadout:setup.loadouts){for(const auto& slot:loadout.slots){result.bindings.push_back({loadout.fighter_id,slot.position,slot.active_skill.value_or("-"),slot.passive?slot.passive->id:"-"});}}
    return result;
}
inline void validate_manifest(const EncounterManifest& value){
    if(value.schema!=1){throw ManifestError("Schema de cenario nao suportado.");}
    const auto expected=reference_manifest(value.profile);
    if(value.scenario!=expected.scenario){throw ManifestError("ID/versao do cenario desconhecido.");}
    if(value.roster!=expected.roster){throw ManifestError("IDs, ordem ou atributos divergentes do roster de referencia.");}
    if(value.skills!=expected.skills){throw ManifestError("IDs/owners de habilidades divergentes.");}
    if(value.bindings!=expected.bindings){throw ManifestError("Posicoes, ativas ou passivas divergentes do catalogo.");}
}
inline std::string encode_manifest(const EncounterManifest& value){
    validate_manifest(value);std::ostringstream out;out.imbue(std::locale::classic());
    out<<"RPG_ENCOUNTER 1\nscenario "<<value.scenario<<"\nprofile "<<(value.profile==CombatProfile::Team?"team":"classic")<<"\nroster "<<value.roster.size()<<'\n';
    for(const auto& unit:value.roster){out<<unit.id<<' '<<(unit.team==Team::A?"A":"B")<<' '<<unit.max_health<<' '<<unit.attack<<' '<<unit.speed<<'\n';}
    out<<"skills "<<value.skills.size()<<'\n';for(const auto& skill:value.skills){out<<skill.first<<' '<<skill.second<<'\n';}
    out<<"bindings "<<value.bindings.size()<<'\n';for(const auto& binding:value.bindings){out<<binding.owner<<' '<<binding.slot<<' '<<binding.active<<' '<<binding.passive<<'\n';}
    out<<"end\n";return out.str();
}
inline EncounterManifest decode_manifest(std::string_view text){
    if(text.empty() || text.size()>65536){throw ManifestError("Tamanho do manifesto invalido (limite 64 KiB).");}
    std::istringstream in{std::string(text)};in.imbue(std::locale::classic());
    const auto token=[&](){std::string s;if(!(in>>s) || s.size()>128){throw ManifestError("Manifesto truncado/token invalido.");}return s;};
    const auto expect=[&](std::string_view s){if(token()!=s){throw ManifestError("Campo de manifesto inesperado.");}};
    const auto number=[&](){const auto s=token();std::int64_t n=0;auto [end,error]=std::from_chars(s.data(),s.data()+s.size(),n);if(error!=std::errc{} || end!=s.data()+s.size() || n<0){throw ManifestError("Numero invalido no manifesto.");}return n;};
    const auto count=[&](){const auto n=number();if(n>64){throw ManifestError("Contagem excede limite de 64.");}return static_cast<int>(n);};
    EncounterManifest result;expect("RPG_ENCOUNTER");if(number()!=1){throw ManifestError("Schema de cenario nao suportado.");}result.schema=1;expect("scenario");result.scenario=token();expect("profile");const auto profile=token();if(profile!="team" && profile!="classic"){throw ManifestError("Perfil invalido.");}result.profile=profile=="team"?CombatProfile::Team:CombatProfile::Classic;
    expect("roster");const int units=count();for(int i=0;i<units;++i){auto id=token();auto side=token();if(side!="A" && side!="B"){throw ManifestError("Lado invalido.");}const auto hp=number(),attack=number(),speed=number();if(speed>std::numeric_limits<int>::max()){throw ManifestError("Velocidade fora do dominio int.");}result.roster.push_back({std::move(id),side=="A"?Team::A:Team::B,hp,attack,static_cast<int>(speed)});}
    expect("skills");const int skills=count();for(int i=0;i<skills;++i){auto owner=token();auto id=token();result.skills.emplace_back(std::move(owner),std::move(id));}
    expect("bindings");const int bindings=count();for(int i=0;i<bindings;++i){auto owner=token();const auto slot=number();if(slot<1 || slot>3){throw ManifestError("Slot de habilidade fora de 1..3.");}auto active=token();auto passive=token();result.bindings.push_back({std::move(owner),static_cast<std::uint32_t>(slot),std::move(active),std::move(passive)});}
    expect("end");std::string extra;if(in>>extra){throw ManifestError("Conteudo extra apos o manifesto.");}validate_manifest(result);return result;
}
inline EncounterSetup compose_manifest(const EncounterManifest& value){validate_manifest(value);return value.profile==CombatProfile::Team?team_reference_setup():classic_reference_setup();}
} // namespace rpg
