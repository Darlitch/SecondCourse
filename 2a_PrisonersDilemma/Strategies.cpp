#include "Strategies.hpp"

#include <fstream>

void AbstractStrategy::UpdateStrat(std::string fileName) {
    if (fileName != "") {
        std::string line;
        std::ifstream file(fileName);
        if (file.is_open()) {
            while (std::getline(file, line)) {
                stratMap[line.substr(0, 3)] = line.substr(4);
            }
            file.close();
        }
    }
}

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

void Strat1::UpdateStrat(std::string fileName) {
    AbstractStrategy::UpdateStrat(fileName);
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

void Strat2::UpdateStrat(std::string fileName) {
    AbstractStrategy::UpdateStrat(fileName);
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

void Strat3::UpdateStrat(std::string fileName) {
    AbstractStrategy::UpdateStrat(fileName);
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

char StratCfg::Move(std::string lastStep, std::size_t plr) {
    char c;
    if (lastStep == "") {
        lastStep = "www";
    }
    std::string line = stratMap[lastStep];
    c = line[plr];
    std::cout << "c:" << c << std::endl;
    return c;
}

void StratCfg::UpdateStrat(std::string fileName) {
    AbstractStrategy::UpdateStrat(fileName);
}

namespace {
AbstractStrategy* CreateStratCfg() {
    return new StratCfg;
}
bool RegCfg() {
    return StratFactory<std::string, AbstractStrategy>::GetInstance()
        ->RegisterStrat("stratCfg", CreateStratCfg);
}
const bool stratCfg = RegCfg();
}  // namespace