#include "CSVfile.hpp"

#include <cctype>

CSVFile::~CSVFile() {
    input.clear();
    table.clear();
}

bool CSVFile::Input(std::string fileName) {
    std::ifstream file(fileName);
    std::string str;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            input.push_back(str);
        }
    } else {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }
    return 0;
}

bool CSVFile::StringProcessing() {
    std::string word = "";
    for (const auto& str : input) {
        for (char const& c : str) {
            if (isalnum(c)) {
                word += c;
            } else {
                if (table.find(word) == table.end()) {
                    table[word] = 1;
                } else {
                    table[word]++;
                }
                word = "";
            }
        }
    }
}