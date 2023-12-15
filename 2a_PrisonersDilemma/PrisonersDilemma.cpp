#include "PrisonersDilemma.hpp"

#include <fstream>
#include <string>

PrisonersDilemma::PrisonersDilemma(std::size_t steps, std::vector<std::string> cfg) : steps_{steps} {
    scores.resize(cfg.size() - 2, 0);
    currPlr.push_back(0);
    currPlr.push_back(1);
    currPlr.push_back(2);
    for (std::size_t i = 2; i < cfg.size(); ++i) {
        AbstractStrategy* strat = StratFactory<std::string, AbstractStrategy>::GetInstance()
                                      ->CreateStrat(cfg[i]);
        strats.push_back(strat);
    }
}

PrisonersDilemma::~PrisonersDilemma() {
    scores.resize(0);
    currPlr.resize(0);
    for (std::size_t i = 0; i < strats.size(); ++i) {
        delete strats[i];
    }
    strats.resize(0);
}

void PrisonersDilemma::Game(std::vector<std::string> cfg, std::string mode) {
    CreateMatrix(cfg[1]);
    if (mode == "detailed") {
        DetailedGame();
    } else if (mode == "fast") {
        FastGame();
    } else if (mode == "tournament") {
        TournamentGame();
    }
    PrintWinner(cfg);
}

void PrisonersDilemma::OneStep(std::string& lastStep) {
    std::string currStep;
    for (auto it : currPlr) {
        currStep.push_back(strats[it]->Move(lastStep, it));
    }
    AddScoreFromMatrix(currStep);
    lastStep = currStep;
}

void PrisonersDilemma::DetailedGame() {
    std::string lastStep;
    for (std::size_t i = 0; i < steps_; ++i) {
        std::cout << scores[0] << ' ' << scores[1] << ' ' << scores[2] << std::endl;
        OneStep(lastStep);
        std::cout << "step " << i+1 << ":" << std::endl;
        std::cout << lastStep[0] << ' ' << lastStep[1] << ' ' << lastStep[2] << std::endl;
        std::cout << scores[0] << ' ' << scores[1] << ' ' << scores[2] << std::endl;
    }
}

void PrisonersDilemma::FastGame() {
    std::string lastStep;
    for (std::size_t i = 0; i < steps_; ++i) {
        OneStep(lastStep);
    }
}

void PrisonersDilemma::TournamentGame() {
    std::cout << "TournamentGame!!!" << std::endl;
}

void PrisonersDilemma::CreateMatrix(std::string matrixName) {
    std::string line;
    std::ifstream file(matrixName);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            matrix[line.substr(0, 3)] = std::stoi(line.substr(3));
        }
        file.close();
    }
}

void PrisonersDilemma::AddScoreFromMatrix(std::string step) {
    int scoreLine = matrix[step];
    scores[currPlr[0]] += scoreLine / 100;
    scores[currPlr[1]] += (scoreLine / 10) % 10;
    scores[currPlr[2]] += scoreLine % 10;
}

void PrisonersDilemma::PrintWinner(std::vector<std::string> cfg) {
    std::size_t winner = 0;
    std::size_t maxScore = 0;
    for (std::size_t i = 0; i < scores.size(); ++i) {
        if (scores[i] > maxScore) {
            maxScore = scores[i];
            winner = i;
        }
    }
    std::cout << "Player(" << winner << "): " << cfg[winner + 2] << " WIN!" << std::endl;
}