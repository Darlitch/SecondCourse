#ifndef PRISONERS_DILEMMA_HPP_
#define PRISONERS_DILEMMA_HPP_

#include <iostream>
#include <vector>
#include <map>

#include "Strategies.hpp"

class PrisonersDilemma {
   public:
    PrisonersDilemma(){};
    PrisonersDilemma(std::size_t steps, std::vector<std::string> cfg);
    PrisonersDilemma(PrisonersDilemma& pd) = delete;
    ~PrisonersDilemma();

    PrisonersDilemma& operator=(PrisonersDilemma& pd) = delete;

    void Game(std::vector<std::string> cfg, std::string mode);
    void OneStep(std::string& lastStep);
    void DetailedGame();
    void FastGame();
    void TournamentGame();

    void CreateMatrix(std::string matrixName);
    void AddScoreFromMatrix(std::string step);

    void PrintWinner(std::vector<std::string> cfg);

   private:
    std::map<std::string, int> matrix;
    std::vector<AbstractStrategy*> strats;
    std::vector<std::size_t> scores;
    std::vector<std::size_t> currPlr;
    std::size_t steps_;
};

#endif