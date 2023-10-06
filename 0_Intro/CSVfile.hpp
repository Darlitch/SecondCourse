#ifndef CSVFILE_HPP_
#define CSVFILE_HPP_

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, std::size_t> map;
typedef std::vector<std::pair<int, std::string>> vector_pair;

class CSVFile {
   public:
    CSVFile() : countWord{0} {};
    ~CSVFile();
    bool Input(std::string fileName);
    bool StringProcessing(std::string str);
    void AddWord(std::string word);
    void ConvertToCSV();
    vector_pair SortingTable();

   private:
    std::size_t countWord;
    map table;
};

#endif