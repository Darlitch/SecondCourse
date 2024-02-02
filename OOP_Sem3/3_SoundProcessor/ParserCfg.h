#ifndef SOUNDPROCESSOR_PARSERCONFIGFILE_H
#define SOUNDPROCESSOR_PARSERCONFIGFILE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <climits>

#include "Exceptions.h"

using std::ifstream;
using std::string;
using std::getline;
using std::vector;
using std::cout;
using std::endl;

struct COMMANDS{
    string cmd;
    string _firstArgument;
    string _secondArgument;
};

class parserConfigFile {
    vector<COMMANDS> commands;
public:
    void preProcessing(const string& configFile);

    vector<COMMANDS>& getCommands();
};


#endif //SOUNDPROCESSOR_PARSERCONFIGFILE_H
