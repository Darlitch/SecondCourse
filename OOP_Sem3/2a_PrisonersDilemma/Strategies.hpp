#ifndef STRATEGIES_HPP_
#define STRATEGIES_HPP_

#include "StratFactory.hpp"

class AbstractStrategy {
   public:
    AbstractStrategy() {}
    virtual ~AbstractStrategy() {}
    virtual void UpdateStrat(std::string fileName = "");
    virtual char Move(std::string lastStep, std::size_t plr) = 0;

   protected:
    std::map<std::string, std::string> stratMap;
};

class Strat1 : public AbstractStrategy {
   public:
    Strat1() {}
    ~Strat1() {}
    void UpdateStrat(std::string fileName) override;
    char Move(std::string lastStep, std::size_t plr);
};

class Strat2 : public AbstractStrategy {
   public:
    Strat2() {}
    ~Strat2() {}
    void UpdateStrat(std::string fileName) override;
    char Move(std::string lastStep, std::size_t plr);
};

class Strat3 : public AbstractStrategy {
   public:
    Strat3() {}
    ~Strat3() {}
    void UpdateStrat(std::string fileName) override;
    char Move(std::string lastStep, std::size_t plr);
};

class StratCfg : public AbstractStrategy {
   public:
    StratCfg() {}
    ~StratCfg() {}
    void UpdateStrat(std::string fileName) override;
    char Move(std::string lastStep, std::size_t plr);
};

#endif