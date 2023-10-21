#ifndef BASE_HPP_
#define BASE_HPP_

#include <iostream>
#include <map>

class Base {
   public:
    char getCondition() {
        return condition;
    }
    virtual void change() = 0;

   protected:
    static std::map<char, Base *(*)()> rules;
    char condition;
};

class T : Base {
    
};

#endif
