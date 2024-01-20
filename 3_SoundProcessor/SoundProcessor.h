#ifndef SOUNDPROCESSOR_SOUNDPROCESSOR_H
#define SOUNDPROCESSOR_SOUNDPROCESSOR_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "ParserCfg.h"
#include "ParserAudio.h"
#include "Exceptions.h"

using std::string;
using std::ifstream;
using std::stoi;
using std::ifstream;
using std::ofstream;
using std::strcmp;
using std::cout;
using std::endl;

class soundProcessor {
    parserConfigFile pCF;
    parserAudio pA;
    ifstream inStream;
public:
    void start(int argc, char* argv[],converterFactory& factory);

};


#endif //SOUNDPROCESSOR_SOUNDPROCESSOR_H
