#include "CoreProbe.h"
#include <iostream>
int main(){
    const auto result=rpg_probe::Run();
    std::cout<<"RPG_CORE_PROBE passed="<<(result.passed?1:0)<<" checkpoints="<<result.checkpoints
        <<" failures="<<result.failures<<" accepted="<<result.accepted<<" credits="<<result.credits<<'\n';
    return result.passed?0:1;
}
