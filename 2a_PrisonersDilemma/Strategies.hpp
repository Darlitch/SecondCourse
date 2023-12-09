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
    virtual ~Strat1() {}
    virtual bool Move();
};

class Strat2 : public AbstractStrategy {
   public:
    Strat2() {}
    virtual ~Strat2() {}
    virtual bool Move();
};

class Strat3 : public AbstractStrategy {
   public:
    Strat3() {}
    virtual ~Strat3() {}
    virtual bool Move();
};

class StratCfg : public AbstractStrategy {
   public:
    StratCfg() {}
    virtual ~StratCfg() {}
    virtual bool Move();
};

#endif