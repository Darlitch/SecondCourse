#include "SoundProcessor.hpp"

#include <string>

SoundProcessor::SoundProcessor(int& argc, char** argv) {
    std::string line = argv[1];
    if (line == "-c") {
        line = argv[2];
        if (!line.find(".txt")) {
            throw ArgumentException("Invalid Config file format: .txt file expected");
        }
        cfg = CfgReader(line);
        for (int i = 3; i < argc; ++i) {
            if (!line.find(".wav")) {
                throw ArgumentException("Invalid file format: .wav file expected");
            }
            files.push_back(line);
        }
        factory = new ConvertFactory;
    } else if (line == "-h") {
        PrintInfo();
    } else {
        throw ArgumentException("Invalid Argument");
    }
}

void SoundProcessor::Processing() {
}

void SoundProcessor::PrintInfo() {}

void SoundProcessor::CopyFiles(std::string in, std::ostream& out) {}
