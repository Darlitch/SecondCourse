#include <sstream>
#include <stdexcept>

#include "PrisonersDilemma.hpp"

std::size_t MenuDetails(int argc, char** argv, std::vector<std::string>& cfg, std::string mode) {
    int steps = 0;
    for (std::size_t i = 1; i <= argc; ++i) {
        std::string currArg(argv[i]);
        if (currArg.find("--mode") != std::string::npos) {
            mode.clear();
            mode.append(argv[i] + 7);
        } else if (currArg.find("--configs=") != std::string::npos) {
            cfg[0].clear();
            cfg[0].append(argv[i] + 10);
        } else if (currArg.find("--matrix=") != std::string::npos) {
            cfg[1].clear();
            cfg[1].append(argv[i] + 9);
        } else if (currArg.find("--steps=-") != std::string::npos) {
            throw std::out_of_range("Incorrect number of steps!");
        } else if (currArg.find("--steps=") != std::string::npos) {
            std::stringstream stream(argv[i] + 8);
            stream >> steps;
        } else {
            cfg.push_back(argv[i]);
        }
    }
    return steps;
}

void CheckInput(std::size_t size, std::size_t steps, std::string mode) {
    if (mode != "detailed" && mode != "fast" && mode != "tournament") {
        throw std::out_of_range("Wrong mode!");
    } else if ((mode == "detailed" || mode == "fast") && size != 5) {
        throw std::out_of_range("Wrong number of strategies!");
    } else if (mode == "tournament" && (size > 12 || size < 6)) {
        throw std::out_of_range("Wrong number of strategies!");
    } else if (steps == 0) {
        throw std::out_of_range("Incorrect number of steps!");
    }
}

int main(int argc, char* argv[]) {
    std::vector<std::string> cfg = {"0", "0"}; 
    std::string mode = "detailed";
    std::size_t steps = 5;
    try {
        steps = MenuDetails(argc, argv, cfg, mode);
        CheckInput(cfg.size(), steps, mode);
    } catch (std::string str) {
        std::cout << str << std::endl;
        return 1;
    }

    PrisonersDilemma game(steps, cfg);
    game.Game(cfg, mode);
    return 0;
}