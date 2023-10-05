#ifndef CSVFILE_HPP_
#define CSVFILE_HPP_

#include <fstream>
#include <iostream>
#include <list>
#include <map>

typedef std::list<std::string> list;
typedef std::map<std::string, std::size_t> map;

class CSVFile {
   public:
    CSVFile(){};
    ~CSVFile();
    bool Input(std::string fileName);
    bool StringProcessing();

   private:
    list input;
    map table;
};

#endif