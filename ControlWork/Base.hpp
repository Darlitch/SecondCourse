#ifndef BASE_HPP_
#define BASE_HPP_

#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <iterator>
#include <list>
#include <map>

class Base {
   public:
    char getCondition() {
        return condition;
    }
    virtual void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        srand(time(NULL));
        int rand = 1 + std::rand() % 2;
        // int rand = 1;п
        switch (rand) {
        case 1:
            Rule1(sequence, it);
            break;
        case 2:
            Rule2(sequence, it);
            break;
        default:
            break;
        }
    }
    virtual void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) = 0;
    virtual void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) = 0;

   protected:
    char condition;
};

class T : public Base {
   public:
    T(char cond) {
        condition = cond;
    }
    void change(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {}
    void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {}
    void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {}
};

class A : public Base {
   public:
    A() {
        condition = 'A';
    }
    void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        sequence.insert(it, new T('a'));
        sequence.insert(it, new T('b'));
        sequence.insert(next(it, 1), new T('b'));
        sequence.insert(next(it, 2), new T('a'));
        // it = next(it, 2);
    }
    void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        sequence.insert(next(it, 1), new T(' '));
        delete *it;
        it = sequence.erase(it);
    }
};

class B : public Base {
   public:
    B() {
        condition = 'B';
    }
    void Rule1(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        sequence.insert(next(it, 1), new A);
        // it = next(it, 1);
    }
    void Rule2(std::list<Base*>& sequence, std::list<Base*>::iterator& it) {
        sequence.insert(next(it, 1), new T('.'));
        delete *it;
        it = sequence.erase(it);
    }
};

#endif  // BASE_HPP_
