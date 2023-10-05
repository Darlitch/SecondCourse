#include <fstream>
#include <iostream>
#include <list>
#include <map>

typedef std::list<std::string> list;
typedef std::map<std::string, std::size_t> map;

int main(int argc, char* argv[]) {
    list input;
    map table;
    if (Input(argv[1], input) == 0) {
    }
}

bool Input(std::string fileName, list& input) {
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

bool StringProcessing(list& input, map& table) {
    std::string word = "";
    for (const auto& str : input) {
        for (char const& c : str) {
            if (c) {
                word+=c;
            } else {
                
            }
        }
    }
}
