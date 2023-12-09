#include <sstream>
#include <stdexcept>

#include "PrisonersDilemma.hpp"

std::size_t MenuDetails(int argc, char** argv, std::vector<std::string>& strats, std::string mode) {
    int steps = 0;
    for (std::size_t i = 1; i <= argc; ++i) {
        std::string currArg(argv[i]);
        if (currArg.find("--mode") != std::string::npos) {
            mode.clear();
            mode.append(argv[i] + 7);
        } else if (currArg.find("--configs=") != std::string::npos) {
            strats[1].clear();
            strats[1].append(argv[i] + 10);
        } else if (currArg.find("--matrix=") != std::string::npos) {
            strats[2].clear();
            strats[2].append(argv[i] + 9);
        } else if (currArg.find("--steps=-") != std::string::npos) {
            throw std::out_of_range("Incorrect number of steps!");
        } else if (currArg.find("--steps=") != std::string::npos) {
            std::stringstream stream(argv[i] + 8);
            stream >> steps;
        } else {
            strats.push_back(argv[i]);
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
    std::vector<std::string> strats = {"0", "0"}; 
    std::string mode = "detailed";
    std::size_t steps = 5;
    try {
        steps = MenuDetails(argc, argv, strats, mode);
        CheckInput(strats.size(), steps, mode);
    } catch (std::string str) {
        std::cout << str << std::endl;
        return 1;
    }

    PrisonersDilemma game(strats.size() - 2, steps);
    game.Game(strats, mode);
    return 0;
}