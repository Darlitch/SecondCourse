#include "CSVfile.hpp"

int main(int argc, char* argv[]) {
    CSVFile converter;
    if (converter.Input(argv[1]) == 0) {
        converter.ConvertToCSV(argv[2]);
    }
    return 0;
}

