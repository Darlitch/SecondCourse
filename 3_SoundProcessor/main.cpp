#include <iostream>
#include <vector>

#include "Exeption.hpp"
#include "SoundProcessor.hpp"


int main(int argc, char** argv) {
    try {
        SoundProcessor proc(argc, argv);
        proc.Processing();
    } catch (const ArgumentException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const UnsupportedFormatException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const FileOpenException& exept) {
        std::cerr << "ERROR: " << exept.what() << std::endl;
        return EXIT_FAILURE;
    }
}