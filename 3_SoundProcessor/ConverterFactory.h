#ifndef SOUNDPROCESSOR_FACTORY_H
#define SOUNDPROCESSOR_FACTORY_H

#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "Exceptions.h"

using std::ifstream;
using std::ofstream;
using std::string;

class converter {
   public:
    virtual void process(int firstSec, int secondSec, ifstream &fFile, ifstream &sFile, ofstream &resultStream) = 0;
};

class mute : public converter {
   public:
    void process(int firstSec, int secondSec, std::ifstream &fFile, std::ifstream &sFile, ofstream &resultStream) override;
};

class mix : public converter {
   public:
    void process(int firstSec, int secondSec, std::ifstream &fFile, std::ifstream &sFile, ofstream &resultStream) override;
};

class addVolume : public converter {
   public:
    void process(int firstSec, int secondSec, std::ifstream &fFile, std::ifstream &sFile, ofstream &resultStream) override;
};

class abstractConverterCreator {
   public:
    virtual converter *create() const = 0;
};

template <class C>
class converterCreator : public abstractConverterCreator {
   public:
    virtual converter *create() const { return new C(); }
};

class converterFactory {
   protected:
    typedef std::map<std::string, abstractConverterCreator *> FactoryMap;
    FactoryMap _factory;

   public:
    converterFactory() = default;

    template <class C>
    void add(const std::string &id) {
        typename FactoryMap::iterator it = _factory.find(id);
        if (it == _factory.end())
            _factory[id] = new converterCreator<C>();
    }

    converter *create(const std::string &id) {
        typename FactoryMap::iterator it = _factory.find(id);
        if (it != _factory.end())
            return it->second->create();
        return 0;
    }
};

#endif  // SOUNDPROCESSOR_FACTORY_H
