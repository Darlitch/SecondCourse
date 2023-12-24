#include "CfgReader.hpp"

#include "Exeption.hpp"

bool CfgReader::OpenFile() {
    cfg.open(fileName_);
    if (!cfg.is_open()) {
        throw FileOpenException(fileName_);
    }
}  // ?

bool CfgReader::CloseFile() {
    cfg.close();
}

std::vector<std::string> CfgReader::GetCfg() {
    std::vector<std::string> param;
    std::string line;
    std::getline(cfg, line);
    while (line[0] == '#') {
        std::getline(cfg, line);
    }

    std::istringstream iss(line);
    std::string temp;
    while (iss >> temp) {
        param.push_back(temp);
    }
    return param;
}

bool CfgReader::GetEOF() {
    return cfg.eof();
}
