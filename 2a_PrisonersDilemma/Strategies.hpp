#ifndef STRATEGIES_HPP_
#define STRATEGIES_HPP_

#include "StratFactory.hpp"

class AbstractStrategy {
   public:
    AbstractStrategy() {}
    virtual ~AbstractStrategy() {}
    virtual bool Move() = 0;
};

class Strat1 : public AbstractStrategy {
   public:
    Strat1() {}
    ~Strat1() {}
    bool Move();
};

class Strat2 : public AbstractStrategy {
   public:
    Strat2() {}
    ~Strat2() {}
    bool Move();
};

class Strat3 : public AbstractStrategy {
   public:
    Strat3() {}
    ~Strat3() {}
    bool Move();
};

class StratCfg : public AbstractStrategy {
   public:
    StratCfg() {}
    ~StratCfg() {}
    bool Move();
};

#endif