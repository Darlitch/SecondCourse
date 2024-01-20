#include "SoundProcessor.h"
#include "ConverterFactory.h"

int main (int argc, char* argv[]) {

    try {
        converterFactory factory;
        factory.add<mute>("mute");
        factory.add<mix>("mix");
        factory.add<addVolume>("add");
        soundProcessor sound;
        sound.start(argc, argv,factory);
    }
    catch (const unsupportedFormatException& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const fileOpenException& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;

}