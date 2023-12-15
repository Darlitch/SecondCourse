#include "Strategies.hpp"

bool Strat1::Move() {
    std::cout << "Strat1!!!" << std::endl;
}

namespace {
AbstractStrategy* CreateStrat1() {
    return new Strat1;
}

bool Reg1() {}
}  // namespace

bool Strat2::Move() {
    std::cout << "Strat2!!!" << std::endl;
}

namespace {
AbstractStrategy* CreateStrat2() {
    return new Strat2;
}

bool Reg2() {}
}  // namespace

bool Strat3::Move() {
    std::cout << "Strat3!!!" << std::endl;
}

namespace {
AbstractStrategy* CreateStrat3() {
    return new Strat3;
}

bool Reg3() {}
}  // namespace

bool StratCfg::Move() {
    std::cout << "StratCfg!!!" << std::endl;
}

namespace {
AbstractStrategy* CreateStratCfg() {
    return new StratCfg;
}

bool RegCfg() {}
}  // namespace