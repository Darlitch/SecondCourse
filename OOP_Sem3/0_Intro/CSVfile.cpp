#include "CSVfile.hpp"

#include <algorithm>
#include <cctype>

namespace Sorting {
bool SortByVal(const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
    return (a.first > b.first);
}
}  // namespace Sorting

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

void CSVFile::StringProcessing(std::string str) {
    std::string word;
    for (const auto c : str) {
        if (isalnum(c)) {
            word.push_back(c);
        } else if (!word.empty()) {
            AddWord(word);
            word.resize(0);
        }
    }
    if (!word.empty()) {
        AddWord(word);
        word.resize(0);
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

void CSVFile::ConvertToCSV(std::string fileName) {
    vector_pair sort_table = SortingTable();
    std::sort(sort_table.begin(), sort_table.end(), Sorting::SortByVal);
    std::ofstream outFile;
    outFile.open(fileName);
    for (const auto& it : sort_table) {
        outFile << it.second << ";" << it.first << ";" << float(it.first) / float(countWord) * 100 << "%\n";
    }
}

vector_pair CSVFile::SortingTable() {
    vector_pair sort;
    for (const auto& it : table) {
        sort.push_back(std::make_pair(it.second, it.first));
    }
    return sort;
}
