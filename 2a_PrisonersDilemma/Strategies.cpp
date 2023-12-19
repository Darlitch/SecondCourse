#include "Strategies.hpp"

char Strat1::Move(std::string lastStep, std::size_t plr) {
    char c = 'c';
    switch (plr) {
    case 0:
        if (lastStep == "cdd") {
            c = 'd';
        }
        break;
    case 1:
        if (lastStep == "dcd") {
            c = 'd';
        }
        break;
    case 2:
        if (lastStep == "ddc") {
            c = 'd';
        }
        break;
    default:
        break;
    }
    return c;
}

namespace {
AbstractStrategy* CreateStrat1() {
    return new Strat1;
}
bool Reg1() {
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat1", CreateStrat1);
}
const bool strat1 = Reg1();
}  // namespace

char Strat2::Move(std::string lastStep, std::size_t plr) {
    char c = 'd';
    if (lastStep == "ddd") {
        c = 'c';
    } else if (lastStep == "ddc" && (plr == 1 || plr == 0)) {
        c = 'c';
    }
    return c;
}

namespace {
AbstractStrategy* CreateStrat2() {
    return new Strat2;
}
bool Reg2() {
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat2", CreateStrat2);
}
const bool strat2 = Reg2();
}  // namespace

char Strat3::Move(std::string lastStep, std::size_t plr) {
    char c = 'c';
    if (lastStep[plr] == c) {
        c = 'd';
    }
    return c;
}

namespace {
AbstractStrategy* CreateStrat3() {
    return new Strat3;
}
bool Reg3() {
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat3", CreateStrat3);
}
const bool strat3 = Reg3();
}  // namespace

void StratCfg::UpdateStrat(std::string fileName) {
}

char StratCfg::Move(std::string lastStep, std::size_t plr) {
    std::cout << "StratCfg!!!" << std::endl;
    return 1;
}

namespace {
AbstractStrategy* CreateStratCfg() {
    return new StratCfg;
}
bool RegCfg() {
    std::cout << "regcfg" << std::endl;
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("stratCfg", CreateStratCfg);
}
const bool stratCfg = RegCfg();
}  // namespace