#ifndef SOUND_PROCESSOR_HPP_
#define SOUND_PROCESSOR_HPP_

#include "CfgReader.hpp"
#include "ConvertFactory.hpp"
#include "Converters.hpp"

class SoundProcessor {
   public:
    SoundProcessor(int& argc, char** argv);  // input?
    void Processing();
    void PrintInfo();

   private:
    void CopyFiles(std::string in, std::ostream& out);

    std::vector<std::string> files;
    CfgReader cfg;
    ConvertFactory* factory;
};

#endif  // SOUND_PROCESSOR_HPP_
