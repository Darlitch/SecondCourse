#include <iostream>
#include <vector>

#include "Exeption.hpp"

int main(int argc, char** argv) {
    std::vector<std::string> files = {"output.wav"};
    std::string line = argv[1];
    try {
        if (line == "-h") {
        } else if (line == "-c") {
        } else {
            throw ArgumentException("Invalid Argument");
        }
    } 
    catch (const ArgumentException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const UnsupportedFormatException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const FileOpenException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    }
}