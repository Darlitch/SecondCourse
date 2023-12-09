#include "PrisonersDilemma.hpp"

PrisonersDilemma::PrisonersDilemma(std::size_t numOfPlayers, std::size_t steps) : steps_{steps}{
    scores.resize(numOfPlayers);
    currPlr.resize(3);
}

PrisonersDilemma::~PrisonersDilemma(){
    scores.resize(0);
    currPlr.resize(0);
}

void Game(std::vector<std::string> strats, std::string mode) {
    if (mode == "detailed") {
        DetailedGame(strats);
    } else if (mode == "fast") {
        FastGame(strats);
    } else if (mode == "tournament") {
        TournamentGame(strats);
    }
}
void DetailedGame(std::vector<std::string> strats);
void FastGame(std::vector<std::string> strats);
void TournamentGame(std::vector<std::string> strats);

void AddScoreFromMatrix(std::string matrixName);