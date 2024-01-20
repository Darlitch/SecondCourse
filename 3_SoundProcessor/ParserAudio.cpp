#include "ParserAudio.h"

void parserAudio::processing(const string& cmd, int firstSec, int secondSec, ifstream& fFile, ifstream& sFile, ofstream& resultStream, converterFactory& factory) {
    cout << "cmd = " << cmd << endl;
    converter* convert = factory.create(cmd);
    if (cmd == "mute" || cmd == "mix" || cmd == "add") {}
    else unsupportedFormatException("unknown command");

    convert->process(firstSec, secondSec, fFile, sFile, resultStream);

    fFile.close();
    sFile.close();
    resultStream.close();

}
