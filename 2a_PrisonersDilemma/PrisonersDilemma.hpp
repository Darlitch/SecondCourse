#ifndef PRISONERS_DILEMMA_HPP_
#define PRISONERS_DILEMMA_HPP_

#include <iostream>
#include <vector>

#include "Strategies.hpp"

class PrisonersDilemma {
   public:
    PrisonersDilemma(){};
    PrisonersDilemma(std::size_t numOfPlayers, std::size_t steps);
    PrisonersDilemma(PrisonersDilemma& pd) = delete;
    ~PrisonersDilemma();

    PrisonersDilemma& operator=(PrisonersDilemma& pd) = delete;

    void Game(std::vector<std::string> strats, std::string mode);
    void DetailedGame(std::vector<std::string> strats);
    void FastGame(std::vector<std::string> strats);
    void TournamentGame(std::vector<std::string> strats);

    void AddScoreFromMatrix(std::string matrixName);

   private:
    std::vector<std::size_t> scores;
    std::vector<std::size_t> currPlr;
    std::size_t steps_;
};

#endif