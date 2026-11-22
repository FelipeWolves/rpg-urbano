#pragma once
#include "rpg/numeric.hpp"
#include <string_view>
namespace rpg::resolution {
inline constexpr std::uint32_t rng_version=1;
enum class RollChannel:std::uint64_t { Miss=1,Block=2,Critical=3,EffectBase=4,Resist=5 };
struct RollKey {std::uint64_t seed=0,action=0,hit=0,target=0,effect=0;};
[[nodiscard]] inline std::uint64_t mix(std::uint64_t x) noexcept {x+=0x9e3779b97f4a7c15ULL;x=(x^(x>>30))*0xbf58476d1ce4e5b9ULL;x=(x^(x>>27))*0x94d049bb133111ebULL;return x^(x>>31);}
[[nodiscard]] inline std::uint64_t identity(std::string_view text) noexcept {std::uint64_t value=14695981039346656037ULL;for(unsigned char c:text){value=(value^c)*1099511628211ULL;}return value;}
[[nodiscard]] inline std::uint32_t roll(const RollKey& key,RollChannel channel){
    const std::uint64_t base=mix(key.seed+0x410000001ULL)^mix(key.action+0x420000001ULL)^mix(key.hit+0x100000001ULL)^mix(key.target)^mix(key.effect+0x200000001ULL)^mix(static_cast<std::uint64_t>(channel)+0x300000001ULL);
    constexpr std::uint64_t bound=10000,threshold=(std::uint64_t{0}-bound)%bound;
    for(std::uint64_t i=0;;++i){const auto value=mix(base+i);if(value>=threshold){return static_cast<std::uint32_t>(value%bound);}if(i==UINT64_MAX){throw std::overflow_error("RNG rejection stream exhausted");}}
}
inline void chance_domain(numeric::BasisPoints value){if(value<0 || value>10000){throw std::invalid_argument("Chance outside 0..10000 basis points");}}
[[nodiscard]] inline bool chance(numeric::BasisPoints value,const RollKey& key,RollChannel channel){chance_domain(value);return value==10000 || (value!=0 && roll(key,channel)<value);}
enum class DamageKind { Normal,True,Dot,TrueDot,Consequence };
enum class Contact { Connected,Miss,Blocked,Invincible };
struct HitChances {
    DamageKind kind=DamageKind::Normal;
    bool invincible=false;
    numeric::BasisPoints miss=0,block=300,critical=0;
    bool allow_special_block_cap=false;
};
struct HitOutcome {Contact contact=Contact::Connected;bool critical=false;bool operator==(const HitOutcome&) const=default;[[nodiscard]] bool counts_for_hit_passive() const noexcept {return contact==Contact::Connected || contact==Contact::Blocked;}};
[[nodiscard]] inline HitOutcome classify_hit(const HitChances& input,const RollKey& key){
    if(input.kind!=DamageKind::Normal && input.kind!=DamageKind::True && input.kind!=DamageKind::Dot && input.kind!=DamageKind::TrueDot && input.kind!=DamageKind::Consequence){throw std::invalid_argument("Unknown damage kind");}
    chance_domain(input.miss);chance_domain(input.block);chance_domain(input.critical);
    if(input.invincible){return {Contact::Invincible,false};}
    if(input.kind!=DamageKind::Dot && input.kind!=DamageKind::TrueDot && input.kind!=DamageKind::Consequence && chance(input.miss,key,RollChannel::Miss)){return {Contact::Miss,false};}
    const auto cap=input.allow_special_block_cap?10000:5000;
    if(input.kind==DamageKind::Normal && chance(input.block>cap?cap:input.block,key,RollChannel::Block)){return {Contact::Blocked,false};}
    return {Contact::Connected,input.kind==DamageKind::Normal && chance(input.critical,key,RollChannel::Critical)};
}
[[nodiscard]] inline bool debuff_applies(const HitOutcome& hit,bool has_attack,bool immunity,bool irresistible,numeric::BasisPoints base,numeric::BasisPoints accuracy,numeric::BasisPoints resistance,const RollKey& key){
    chance_domain(base);numeric::nonnegative(accuracy);numeric::nonnegative(resistance);
    if(immunity || (has_attack && (hit.contact==Contact::Miss || hit.contact==Contact::Blocked))){return false;}
    if(!chance(base,key,RollChannel::EffectBase)){return false;}
    if(irresistible){return true;}
    const auto difference=resistance>accuracy?resistance-accuracy:0;
    const auto effective=difference<2000?2000:difference>10000?10000:difference;
    return !chance(effective,key,RollChannel::Resist);
}
} // namespace rpg::resolution
