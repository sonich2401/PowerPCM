#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (str.size() == 0 || str.empty())
        return;
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

vector<string> split(string s, const char c)
{
    //std::cout <<"STROMG:"<< s<<'\n';
    std::vector<std::string> ret;
    string tmp = "";
    for (char obj : s) {
        if (obj != c) {
            tmp += obj;

        }
        else {
            ret.push_back(tmp);
            tmp = "";
        }
    }
    ret.push_back(tmp);
    return ret;
}

void remove_og_spaces(std::string& str) {
    string tmp = "";
    for (unsigned int i = 0; i < str.size(); i++) {
        if (str[i] != ' ') {
            for (unsigned int i1 = i; i1 < str.size(); i1++) {
                tmp += str[i1];
            }
            str = tmp;
            return;
        }
    }
}

std::string one_space(std::string str) {
    string tmp = "";
    for (unsigned int i = 0; i < str.size(); i++) {
        if (i - 1 != 0) {
            if (str[i - 1] != ' ' && str[i] == ' ') {
                tmp += ' ';
                continue;
            }
            if (str[i] != ' ') {
                tmp += str[i];
            }
        }
    }
    return tmp;
}

int new_rfind(std::string str, char chr, int offset=0) {
    int ret = -1;
    for (unsigned int i = offset; i < str.size(); i++) {
        if (str[i] == chr)
            return i;
    }
    return ret;
}

std::vector<std::string> get_args(std::string str) {
    std::vector<std::string> ret;
    int pos = 0;
    int next_pos = 0;
    replaceAll(str, "\t", "");

    while (pos != -1) {
        pos = new_rfind(str, ' ', next_pos);
        next_pos = new_rfind(str, ' ', pos + 1);
        if (pos == -1) {
            break;
        }
        if (pos > -1 && next_pos == -1) {
            next_pos = str.size();
        }

        std::string tmp = "";
        for (unsigned int char_pos = pos + 1; char_pos < next_pos; char_pos++) {
            tmp += (str[char_pos]);
        }

        if (tmp.empty()) {
            break;
        }
        ret.push_back(tmp);
    }
    
    return ret;
}