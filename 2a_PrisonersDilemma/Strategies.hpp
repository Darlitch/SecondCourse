#ifndef STRATEGIES_HPP_
#define STRATEGIES_HPP_

#include "StratFactory.hpp"

class AbstractStrategy {
   public:
    AbstractStrategy() {}
    virtual ~AbstractStrategy() {}
    virtual char Move(std::string lastStep, std::size_t plr) = 0;
};

class Strat1 : public AbstractStrategy {
   public:
    Strat1() {}
    ~Strat1() {}
    char Move(std::string lastStep, std::size_t plr);
};

class Strat2 : public AbstractStrategy {
   public:
    Strat2() {}
    ~Strat2() {}
    char Move(std::string lastStep, std::size_t plr);
};

class Strat3 : public AbstractStrategy {
   public:
    Strat3() {}
    ~Strat3() {}
    char Move(std::string lastStep, std::size_t plr);
};

class StratCfg : public AbstractStrategy {
   public:
    StratCfg() {}
    ~StratCfg() {}
    void UpdateStrat(std::string fileName);
    char Move(std::string lastStep, std::size_t plr);

   private:
    std::map<std::string, std::string> stratMap;
};

#endif