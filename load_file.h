#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "string_tools.h"
using namespace std;

std::vector<std::string> load_file(std::string fp) {
    int prg = 0;
    std::vector < string > lines;
    fstream file(fp, std::ios::in);
    string tmp;
    while (!file.eof()) {
        getline(file, tmp);
        //'©'
        //replaceAll(tmp, "©", "");
        lines.push_back(tmp);
    }
    file.close();
    return lines;
}
