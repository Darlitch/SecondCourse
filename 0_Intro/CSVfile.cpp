#include "CSVfile.hpp"

#include <cctype>

CSVFile::~CSVFile() {
    table.clear();
}

bool CSVFile::Input(std::string fileName) {
    std::ifstream file(fileName);
    std::string str;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            StringProcessing(str);
        }
    } else {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }
    return 0;
}

bool CSVFile::StringProcessing(std::string str) {
    std::string word;
    for (const auto c : str) {
        if (isalnum(c)) {
            word += c; // стандартное добавление элемента строки
        } else if (!word.empty()) {
            AddWord(word);
            word.resize(0); 
        }
    }
}

void CSVFile::AddWord(std::string word) {
    if (table.find(word) == table.end()) {
        table[word] = 1;
    } else {
        table[word]++;
    }
    countWord++;
}

void CSVFile::ConvertToCSV() {
    vector_pair sort_table = SortingTable();
}

vector_pair CSVFile::SortingTable() {
}
