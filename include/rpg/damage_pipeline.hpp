#pragma once
#include "rpg/hit_resolution.hpp"
#include <algorithm>
#include <span>
#include <vector>
#include <optional>
namespace rpg::resolution {
namespace detail {
// Bounded by <=16 factors of <=63 bits: fewer than 35 base-2^32 words.
struct Natural {
    std::vector<std::uint32_t> words;
    explicit Natural(std::uint64_t v):words{static_cast<std::uint32_t>(v),static_cast<std::uint32_t>(v>>32)}{trim();}
    void trim(){while(words.size()>1 && words.back()==0){words.pop_back();}}
    [[nodiscard]] Natural times(std::uint64_t value) const {
        Natural out(0);out.words.assign(words.size()+2,0);
        const std::uint32_t parts[2]={static_cast<std::uint32_t>(value),static_cast<std::uint32_t>(value>>32)};
        for(std::size_t i=0;i<words.size();++i){std::uint64_t carry=0;for(std::size_t j=0;j<2;++j){const auto current=static_cast<std::uint64_t>(words[i])*parts[j]+out.words[i+j]+carry;out.words[i+j]=static_cast<std::uint32_t>(current);carry=current>>32;}out.words[i+2]=static_cast<std::uint32_t>(carry);}
        out.trim();return out;
    }
    [[nodiscard]] int compare(const Natural& other) const noexcept {
        if(words.size()!=other.words.size()){return words.size()<other.words.size()?-1:1;}
        for(std::size_t i=words.size();i>0;--i){if(words[i-1]!=other.words[i-1]){return words[i-1]<other.words[i-1]?-1:1;}}return 0;
    }
};
}
struct Factor {std::int64_t numerator,denominator;};
[[nodiscard]] inline numeric::Quantity product_floor(std::int64_t base,std::span<const Factor> factors,numeric::Quantity cap=numeric::quantity_max){
    numeric::nonnegative(base);numeric::nonnegative(cap);if(cap>numeric::quantity_max || factors.size()>16){throw std::invalid_argument("Invalid product limits");}
    detail::Natural numerator(static_cast<std::uint64_t>(base)),denominator(1);
    for(const auto& f:factors){numeric::nonnegative(f.numerator);if(f.denominator<=0){throw std::invalid_argument("Invalid factor divisor");}numerator=numerator.times(static_cast<std::uint64_t>(f.numerator));denominator=denominator.times(static_cast<std::uint64_t>(f.denominator));}
    if(numerator.compare(denominator.times(static_cast<std::uint64_t>(cap)))>=0){return cap;}
    std::uint64_t low=0,high=static_cast<std::uint64_t>(cap);
    while(low+1<high){const auto mid=low+(high-low)/2;if(denominator.times(mid).compare(numerator)<=0){low=mid;}else{high=mid;}}
    return static_cast<numeric::Quantity>(low);
}
enum class CombatModel { Legacy, FunnelV1 };
struct FighterStats {
    std::optional<numeric::Quantity> defense;
    numeric::BasisPoints block=300,critical_chance=0,critical_total=15000,critical_resistance=0;
    numeric::BasisPoints miss=0,evasion=0,accuracy=0,resistance=0,defense_ignore=0,damage_bonus=0,mitigation=0;
    bool special_block_cap=false,special_mitigation_cap=false,invincible=false,debuff_immunity=false;
    bool operator==(const FighterStats&) const=default;
};
inline void validate_stats(const FighterStats& s){
    if(s.defense && (*s.defense<0 || *s.defense>numeric::quantity_max)){throw std::invalid_argument("DEF outside quantity domain");}
    for(auto value:{s.block,s.critical_chance,s.critical_resistance,s.miss,s.evasion,s.defense_ignore}){chance_domain(value);}
    for(auto value:{s.critical_total,s.accuracy,s.resistance,s.damage_bonus,s.mitigation}){if(value<0 || value>numeric::quantity_max){throw std::invalid_argument("Stat outside nonnegative domain");}}
}
struct DamageInput {
    numeric::Quantity base=0;
    numeric::BasisPoints ratio=10000;
    FighterStats attacker,defender;
    DamageKind kind=DamageKind::Normal;
    bool guarding=false;
};
struct DamageResult {
    HitOutcome outcome;
    numeric::Quantity amount=0;
};
struct HitReport {
    HitOutcome outcome;
    DamageKind kind=DamageKind::Normal;
    numeric::Quantity attempted=0,absorbed=0,health_damage=0,overkill=0;
    bool operator==(const HitReport&) const=default;
};
[[nodiscard]] inline DamageResult calculate_damage(const DamageInput& input,const RollKey& key){
    numeric::nonnegative(input.base);numeric::nonnegative(input.ratio);if(input.ratio>numeric::quantity_max){throw std::invalid_argument("Ratio outside domain");}validate_stats(input.attacker);validate_stats(input.defender);
    HitChances chances;chances.kind=input.kind;chances.invincible=input.defender.invincible;chances.miss=std::min<std::int64_t>(10000,input.attacker.miss+input.defender.evasion);chances.block=input.defender.block;chances.allow_special_block_cap=input.defender.special_block_cap;chances.critical=std::max<std::int64_t>(0,input.attacker.critical_chance-input.defender.critical_resistance);
    const auto outcome=classify_hit(chances,key);
    if(outcome.contact==Contact::Invincible || outcome.contact==Contact::Blocked){return {outcome,0};}
    std::vector<Factor> factors{{input.ratio,10000}};
    if(outcome.contact==Contact::Miss){factors.push_back({7500,10000});}
    if(outcome.critical){factors.push_back({input.attacker.critical_total,10000});}
    if(input.kind!=DamageKind::True && input.kind!=DamageKind::TrueDot && input.kind!=DamageKind::Consequence){
        if(input.defender.defense){factors.push_back({18000000,5700000+*input.defender.defense*(10000-input.attacker.defense_ignore)});}
        const auto cap=input.defender.special_mitigation_cap?10000:5000;
        const auto reduction=std::min<std::int64_t>(cap,input.defender.mitigation);
        factors.push_back({10000+input.attacker.damage_bonus-reduction,10000});
    }
    if(input.kind==DamageKind::True){factors.push_back({10000+input.attacker.damage_bonus,10000});}
    const bool guard=input.kind!=DamageKind::True && input.kind!=DamageKind::TrueDot && input.kind!=DamageKind::Consequence && input.guarding;
    if(guard){factors.push_back({1,2});}
    auto amount=product_floor(numeric::quantity(input.base),factors);
    if(input.kind==DamageKind::Normal && !guard){amount=std::max<numeric::Quantity>(1,amount);}
    return {outcome,amount};
}
} // namespace rpg::resolution
