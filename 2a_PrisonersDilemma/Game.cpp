#include <sstream>
#include <stdexcept>

#include "PrisonersDilemma.hpp"

std::size_t MenuDetails(int argc, char** argv, std::vector<std::string>& strats) {
    int steps = 0;
    for (std::size_t i = 1; i <= argc; ++i) {
        std::string currArg(argv[i]);
        if (currArg.find("--mode") != std::string::npos) {
            strats[0].clear();
            strats[0].append(argv[i] + 7);
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

void CheckInput(std::vector<std::string> strats, std::size_t steps) {
    if (strats[0] != "detailed" && strats[0] != "fast" && strats[0] != "tournament") {
        throw std::out_of_range("Wrong mode!");
    } else if ((strats[0] == "detailed" || strats[0] == "fast") && strats.size() != 6) {
        throw std::out_of_range("Wrong number of strategies!");
    } else if (strats[0] == "tournament" && (strats.size() > 12 || strats.size() < 7)) {
        throw std::out_of_range("Wrong number of strategies!");
    } else if (steps == 0) {
        throw std::out_of_range("Incorrect number of steps!");
    }
}

int main(int argc, char* argv[]) {
    std::vector<std::string> strats = {"detailed", "0", "0"};  // доработать с steps and matrix
    std::size_t steps = 5;
    try {
        steps = MenuDetails(argc, argv, strats);
        CheckInput(strats, steps);
    } catch (std::string str) {
        std::cout << str << std::endl;
        return 1;
    }

    PrisonersDilemma game(strats.size() - 3);
    if (strats[0] == "detailed") {
        game.DetailedGame(strats);
    } else if (strats[0] == "fast") {
        game.FastGame(strats);
    } else {
        game.TournamentGame(strats);
    }
    return 0;
}