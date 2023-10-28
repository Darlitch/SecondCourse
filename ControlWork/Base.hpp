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
    char getCondition() {
        return condition;
    }
    // virtual void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
    //     std::random_device system_rand;
    //     srand(system_rand());
    //     int rand = 1 + std::rand() % 2;
    //     switch (rand) {
    //     case 1:
    //         Rule1(sequence, it);
    //         break;
    //     case 2:
    //         Rule2(sequence, it);
    //         break;
    //     default:
    //         break;
    //     }
    // }
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
        rules[rand].clear();
        delete *it;
        it = next(sequence.erase(it), rules.at(rand).size() - 1);
    }

   protected:
    std::vector<std::list<Base*>> rules;
};

class A : public N {
   public:
    A() {
        condition = 'A';
    }
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
    void RulesInit() {
        rules.push_back({new B, new A});
        rules.push_back({new T('.')});
    }
};

// class A : public Base {
//    public:
//     A() {
//         condition = 'A';
//     }
//     void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
//         std::list<Base*> sequenceNew = {new T('a'), new T('b')};
//         sequence.insert(it, sequenceNew.begin(), sequenceNew.end());
//         sequenceNew.clear();
//         // sequence.insert(it, new T('a'));
//         // sequence.insert(it, new T('b'));
//         std::list<Base*> sequenceNew2 = {new T('b'), new T('a')};
//         sequence.insert(next(it, 1), sequenceNew2.begin(), sequenceNew2.end());
//         // sequence.insert(next(it, 1), new T('b'));
//         // sequence.insert(next(it, 2), new T('a'));
//         sequenceNew2.clear();
//     }
//     void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
//         std::list<Base*> sequenceNew = {new T(' ')};
//         sequence.insert(next(it, 1), sequenceNew.begin(), sequenceNew.end());
//         sequenceNew.clear();
//         delete *it;
//         it = sequence.erase(it);
//     }
// };

// class B : public Base {
//    public:
//     B() {
//         condition = 'B';
//     }
//     void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
//         std::list<Base*> sequenceNew = {new A};
//         sequence.insert(next(it, 1), sequenceNew.begin(), sequenceNew.end());
//         sequenceNew.clear();
//     }
//     void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
//         std::list<Base*> sequenceNew = {new T('.')};
//         sequence.insert(next(it, 1), sequenceNew.begin(), sequenceNew.end());
//         sequenceNew.clear();
//         delete *it;
//         it = sequence.erase(it);
//     }
// };

#endif  // BASE_HPP_
