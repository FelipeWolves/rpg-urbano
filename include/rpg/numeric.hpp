#pragma once
#include <cstdint>
#include <stdexcept>
namespace rpg::numeric {
using Quantity=std::int64_t;
using BasisPoints=std::int64_t;
inline constexpr Quantity quantity_max=1000000000000LL;
inline constexpr BasisPoints basis_point_scale=10000;
inline void nonnegative(std::int64_t value){if(value<0){throw std::invalid_argument("Negative numeric operand");}}
[[nodiscard]] inline Quantity quantity(std::int64_t value){nonnegative(value);return value>quantity_max?quantity_max:value;}
[[nodiscard]] inline Quantity add(Quantity a,Quantity b){nonnegative(a);nonnegative(b);return a>=quantity_max || b>=quantity_max || a>quantity_max-b?quantity_max:a+b;}
// Exact floor(a*b/d), capped only in the quotient domain. No wide integer extension.
// Each binary prefix is represented as q*d+r; remainders are added without overflow.
[[nodiscard]] inline Quantity mul_div_floor(std::int64_t a,std::int64_t b,std::int64_t divisor,Quantity cap=quantity_max){
    nonnegative(a);nonnegative(b);nonnegative(cap);
    if(divisor<=0 || cap>quantity_max){throw std::invalid_argument("Invalid divisor/result cap");}
    const auto d=static_cast<std::uint64_t>(divisor),base_q=static_cast<std::uint64_t>(a)/d,base_r=static_cast<std::uint64_t>(a)%d;
    const auto limit=static_cast<std::uint64_t>(cap),factor=static_cast<std::uint64_t>(b);
    std::uint64_t q=0,r=0;
    for(int bit=62;bit>=0;--bit){
        if(q>limit/2){return cap;}q*=2;
        if(r>=d-r){r-=d-r;if(q==limit){return cap;}++q;}else{r+=r;}
        if((factor>>bit)&1U){
            if(base_q>limit || q>limit-base_q){return cap;}q+=base_q;
            if(r>=d-base_r){r-=d-base_r;if(q==limit){return cap;}++q;}else{r+=base_r;}
        }
        if(q>=limit){return cap;}
    }
    return static_cast<Quantity>(q);
}
[[nodiscard]] inline Quantity percent_floor(Quantity value,BasisPoints rate){return mul_div_floor(value,rate,basis_point_scale);}
// Exact rational comparison by continued fractions, including true equality.
[[nodiscard]] inline int compare_ratio(std::int64_t an,std::int64_t ad,std::int64_t bn,std::int64_t bd){
    nonnegative(an);nonnegative(bn);if(ad<=0 || bd<=0){throw std::invalid_argument("Ratio denominator must be positive");}
    int direction=1;
    for(;;){
        const auto aq=an/ad,bq=bn/bd;
        if(aq!=bq){return (aq<bq?-1:1)*direction;}
        const auto ar=an%ad,br=bn%bd;
        if(ar==0 || br==0){return ar==br?0:(ar==0?-1:1)*direction;}
        an=ad;ad=ar;bn=bd;bd=br;direction=-direction;
    }
}
} // namespace rpg::numeric
