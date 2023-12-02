#ifndef PRISONERS_DILEMMA_HPP_
#define PRISONERS_DILEMMA_HPP_

#include <iostream>
#include <vector>

class PrisonersDilemma {
   public:
    PrisonersDilemma(){};
    PrisonersDilemma(std::size_t numOfPlayers);
    PrisonersDilemma(PrisonersDilemma& pd) = delete;
    ~PrisonersDilemma();

    PrisonersDilemma& operator=(PrisonersDilemma& pd) = delete;

    void DetailedGame(std::vector<std::string> strats);
    void FastGame(std::vector<std::string> strats);
    void TournamentGame(std::vector<std::string> strats);

   private:
};

#endif