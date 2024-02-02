#include "SoundProcessor.h"

bool readWavHeader(parserAudio &file, ifstream& in) {
    in.seekg(0, std::ios::beg);

    if (!in) {
        std::cout << "Failed to open file." << std::endl;
        return false;
    }
    in.read(reinterpret_cast<char *>(&file.header), sizeof(file.header));
    in.seekg(file.header.subchunk_2_size, std::ios::cur);
    in.read(file.header.subchunk_2_id, sizeof(file.header.subchunk_2_id));
    in.read(reinterpret_cast<char *>(&file.header.subchunk_2_size), sizeof(file.header.subchunk_2_size));
    return true;
}

void soundProcessor::start(int argc, char* argv[], converterFactory& factory) {
    bool flag = true;
    ofstream resultFile1;
    ofstream resultFile2;

    if (argc == 2 && !strcmp(argv[1],"-h")){
        cout << "HELP" << endl;
        cout << "mute F S - mute sounds between F and S seconds \n"
                "mix $F S - mix sounds with file F start from S second\n"
                "add F S - increase the volume by S times after F seconds"<< endl;
    }

    if (argc == 4) {
        cout << "not input files" << endl;
        return;
    }

    if (!strcmp(argv[1],"-c") && argc > 2) {
        pCF.preProcessing(argv[2]);
    }
    if (argc > 3) {


        ifstream fFile;
        ifstream sFile;

        string const tmpFile1 = "tmp123123123.wav";
        string const tmpFile2 = "tmp182458125481.wav";

        bool flagFirstIteration = true;

        for (auto i : pCF.getCommands()) {
            if (flag) {
                resultFile1.open(tmpFile1, std::ios::binary | std::ios::trunc);
                resultFile2.close();
                if (!resultFile1) {
                    throw fileOpenException(tmpFile1);
                }
                flag = false;
            } else {
                resultFile2.open(tmpFile2, std::ios::binary | std::ios::trunc);
                if (!resultFile2) {
                    throw fileOpenException(tmpFile2);
                }
                resultFile1.close();
                flag = true;
            }

            ofstream resultStream;

            if (resultFile1.is_open()) {
                resultStream.open(tmpFile1, std::ios::binary | std::ios::trunc);
                if (!resultStream.is_open()) {
                    throw fileOpenException(tmpFile1);
                }
            }
            else {
                resultStream.open(tmpFile2, std::ios::binary | std::ios::trunc);
                if (!resultStream.is_open()) {
                    throw fileOpenException(tmpFile2);
                }
            }

            int firstSec = 0;
            int numFirstFile = 0;
            int secondSec;
            if (i._firstArgument.find('$') == ULLONG_MAX) { //если доллара нет
                firstSec = stoi(i._firstArgument);
            }
            else {
                numFirstFile = stoi(i._firstArgument.substr(i._firstArgument.find('$') + 1));
            }
            secondSec = stoi(i._secondArgument);

            if (flagFirstIteration) {
                fFile.open(argv[4], std::ios::binary);
                if (!fFile) {
                    throw fileOpenException(argv[4]);
                }
                readWavHeader(pA, fFile);
                pA.processing(i.cmd, firstSec, secondSec, fFile, sFile, resultStream, factory);
                flagFirstIteration = false;
            } else {
                if (numFirstFile != 0) {
                    if (resultFile1.is_open()) fFile.open(tmpFile2, std::ios::binary);
                    else fFile.open(tmpFile1, std::ios::binary);
                    sFile.open(argv[numFirstFile + 3], std::ios::binary);
                    if (!fFile) {
                        throw fileOpenException(argv[4]);
                    }
                    readWavHeader(pA, sFile);
                    pA.processing(i.cmd, firstSec, secondSec, fFile, sFile, resultStream, factory);
                } else {
                    if (resultFile1.is_open()) fFile.open(tmpFile2, std::ios::binary);
                    else fFile.open(tmpFile1, std::ios::binary);
                    pA.processing(i.cmd, firstSec, secondSec, fFile, sFile, resultStream, factory);
                }
            }
            resultStream.close();
        }
        ofstream resultStreamToFile;
        resultStreamToFile.open(argv[3], std::ios::binary | std::ios::trunc);

        if (!resultStreamToFile.is_open()) throw fileOpenException(argv[3]);

        resultStreamToFile.write(reinterpret_cast<char*>(&pA.header), sizeof(pA.header));
        if (resultFile1.is_open()) {
            ifstream tmpStream;

            tmpStream.open(tmpFile1, std::ios::binary);
            if (!tmpStream.is_open()) throw fileOpenException(tmpFile1);
            short tmp = 0;

            while (tmpStream.read((char*)&tmp, sizeof(short))) {
                resultStreamToFile.write((char*)&tmp, sizeof(short));
            }
            resultStreamToFile.write((char*)&tmp, sizeof(short));
            tmpStream.close();
        } else {
            ifstream tmpStream;

            tmpStream.open(tmpFile2, std::ios::binary);
            short tmp = 0;

            while (tmpStream.read((char*)&tmp, sizeof(short))) {
                resultStreamToFile.write((char*)&tmp, sizeof(short));
            }
            resultStreamToFile.write((char*)&tmp, sizeof(short));
            tmpStream.close();
        }
        resultStreamToFile.close();
        resultFile1.close();
        resultFile2.close();
        fFile.close();
        sFile.close();
    }
    
}