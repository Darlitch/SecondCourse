#include "Strategies.hpp"

char Strat1::Move(std::string lastStep, std::size_t plr) {
    std::cout << "Strat1!!!" << std::endl;
    return 1;
}

namespace {
AbstractStrategy* CreateStrat1() {
    return new Strat1;
}
bool Reg1() {
    std::cout << "reg1" << std::endl;
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat1", CreateStrat1);
}
const bool strat1 = Reg1();
}  // namespace

char Strat2::Move(std::string lastStep, std::size_t plr) {
    std::cout << "Strat2!!!" << std::endl;
    return 1;
}

namespace {
AbstractStrategy* CreateStrat2() {
    return new Strat2;
}
bool Reg2() {
    std::cout << "reg2" << std::endl;
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat2", CreateStrat2);
}
const bool strat2 = Reg2();
}  // namespace

char Strat3::Move(std::string lastStep, std::size_t plr) {
    std::cout << "Strat3!!!" << std::endl;
    return 1;
}

namespace {
AbstractStrategy* CreateStrat3() {
    return new Strat3;
}
bool Reg3() {
    std::cout << "reg3" << std::endl;
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("strat3", CreateStrat3);
}
const bool strat3 = Reg3();
}  // namespace

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