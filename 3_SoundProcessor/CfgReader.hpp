#ifndef CFG_READER_HPP_
#define CFG_READER_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class CfgReader {
   public:
    CfgReader() {}
    CfgReader(std::string fileName) : fileName_(fileName) {}
    ~CfgReader() {}

    bool OpenFile();  // ?
    bool CloseFile();
    std::vector<std::string> GetCfg();
    bool GetEOF();

   private:
    std::string fileName_;
    std::ifstream cfg;
};

#endif  // CFG_READER_HPP_