#ifndef BASE_HPP_
#define BASE_HPP_

#include <stdlib.h>

#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>

class Base {
   public:
    virtual ~Base() {}
    char getCondition() {
        return condition;
    }
    virtual void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) = 0;

   protected:
    char condition;
};

class T : public Base {
   public:
    T(char cond) {
        condition = cond;
    }
    void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {}
};

class N : public Base {
   public:
    virtual ~N() {
        for (auto const& it : rules) {
            for (auto const& it2 : it) {
                delete it2;
            }
        }
    }
    virtual void RulesInit() = 0;
    virtual void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        std::random_device system_rand;
        srand(system_rand());
        RulesInit();
        int rand = 0 + std::rand() % rules.size();
        sequence.insert(it, rules.at(rand).begin(), rules.at(rand).end());
        // delete (*it);
        it = sequence.erase(it);
        it = next(it, rules.at(rand).size() - 1);
        rules.at(rand).clear();
    }

   protected:
    std::vector<std::list<Base*>> rules;
};

class A : public N {
   public:
    A() {
        condition = 'A';
    }
    virtual ~A(){};
    void RulesInit() {
        // std::list<Base*> rule1 = {new T('a'), new T('b'), new A, new T('b'), new T('a')};
        // rules.push_back(rule1);
        rules.push_back({new T('a'), new T('b'), new A, new T('b'), new T('a')});
        rules.push_back({new T(' ')});
    }
};

class B : public N {
   public:
    B() {
        condition = 'B';
    }
    virtual ~B() {}
    void RulesInit() {
        rules.push_back({new B, new A});
        rules.push_back({new T('.')});
    }
};

#endif  // BASE_HPP_
