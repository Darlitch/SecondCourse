#include "ConverterFactory.h"

short readSample(ifstream &inStream) {
    short tmp = 0;
    inStream.read((char*)&tmp, sizeof(short));
    return tmp;
}

void writeSample(ofstream &outStream, short& sample) {
    outStream.write((char*)&sample, sizeof(short));
}

void mute::process(int firstSec, int secondSec, std::ifstream &fFile, std::ifstream &sFile, ofstream& resultStream) {
    short tmp1;
    if (!sFile) tmp1 = 0;
    for (int i = 0; i < firstSec * 44100; ++i) {
        short tmp = readSample(fFile);
        writeSample(resultStream, tmp);
    }
    for (int i = 0; i < (secondSec - firstSec) * 44100; ++i) {//TODO: change 44100 on header.sample_rate
        short sample1 = readSample(fFile);
        sample1 = 0;
        writeSample(resultStream, sample1);
    }
    tmp1 = 0;

    while (fFile.read(reinterpret_cast<char*> (&tmp1), sizeof(short))) {
        writeSample(resultStream, tmp1);
    }
    writeSample(resultStream, tmp1);
}

void mix::process(int firstSec, int secondSec, ifstream &fFile, ifstream &sFile, ofstream &resultStream) {
    short tmp1 = 0, tmp2 = 0;
    if(firstSec) tmp1 = 0;
    for (int i = 0; i < secondSec * 44100; ++i) {
        short tmp = readSample(fFile);
        writeSample(resultStream, tmp);
    }
    while (fFile.read((char*)&tmp1, sizeof(short)) && sFile.read((char*)&tmp2, sizeof(short))) {
        short tmp = (tmp1/2 + tmp2/2);
        writeSample(resultStream, tmp);
    }
}

void addVolume::process(int firstSec, int secondSec, ifstream &fFile, ifstream &sFile, ofstream &resultStream) {
    short tmp = 0;
    if (sFile) tmp = 0;

        for (int i = 0; i < firstSec * 44100; ++i) {
        tmp = readSample(fFile);
        writeSample(resultStream, tmp);
    }

    while (fFile.read((char*)&tmp, sizeof(short))) {
        short newSample = tmp * secondSec;
        if (newSample < 0) newSample = SHRT_MAX;
        writeSample(resultStream, newSample);
    }
}
