#include "ParserCfg.h"

void parserConfigFile::preProcessing(const string& configFile) {

    ifstream iStream;
    iStream.open(configFile);

    if (!iStream) {
        throw fileOpenException(configFile);
    }

    string line;
    while (getline(iStream, line)) {
        if (line[0] == '#' || line.length() == 1) continue;
        COMMANDS tmp;
        tmp.cmd = line.substr(0,line.find(' '));
        line = line.substr(line.find(' ') + 1);
        tmp._firstArgument = line.substr(0, line.find(' '));
        tmp._secondArgument = line.substr(line.find(' ') + 1);
        commands.push_back(tmp);
    }
    iStream.close();
}

vector<COMMANDS>& parserConfigFile::getCommands() {
    return commands;
}
