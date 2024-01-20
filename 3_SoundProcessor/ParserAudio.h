#ifndef SOUNDPROCESSOR_PARSERAUDIO_H
#define SOUNDPROCESSOR_PARSERAUDIO_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

#include "ConverterFactory.h"
#include "Exceptions.h"

using std::vector;
using std::string;
using std::size_t;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

struct wav_header{
    char riff[4];
    unsigned int chunk_size;
    char format[4];
    char subchunk_1_id[4];
    unsigned int subchunk_1_size;
    unsigned short audio_format;
    unsigned short num_channels;
    unsigned int sample_rate;
    unsigned int byte_rate;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char subchunk_2_id[4];
    unsigned int subchunk_2_size;
};

class parserAudio {
public:
    wav_header header{};
    void processing(const string& cmd, int firstSec, int secondSec, ifstream& fFile, ifstream& sFile, ofstream& resultStream, converterFactory& factory);
};


#endif //SOUNDPROCESSOR_PARSERAUDIO_H
