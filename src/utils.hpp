#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class BannerUtils {
public:
    static void loadBanner(std::string path = "../res/banner.txt")
    {
        std::ifstream istream;
        istream.open(path, std::ios::in);
        if (!istream.is_open()) {
            return;
        }
        std::string linestr;
        while (std::getline(istream, linestr)){
            std::cout << linestr << std::endl;
        }
        istream.close();
    }
};

class StringUtils {
public:
    static std::vector<std::string> split(std::string str, std::string pattern)
    {
        std::string::size_type pos;
        std::vector<std::string> result;
        str += pattern;
        int size = str.size();
        for (int i = 0; i < size; i++) {
            pos = str.find(pattern, i);
            if (pos < size) {
                std::string s = str.substr(i, pos - i);
                result.push_back(s);
                i = pos + pattern.size() - 1;
            }
        }
        return result;
    }
    static std::string montage(std::vector<std::string> &nodes, std::string str) 
    {
        std::string _str;
        for(int i = 0; i < nodes.size() - 1 ; ++i) {
            _str += nodes.at(i);
            _str += str;
        }
        _str += nodes.back();
        return _str;
    }
};

class ByteUtils {
public:
    static std::string byte_string(long long size) {
        std::string auto_size;
        long double real_size = (long double) size;
        if(real_size < 1024 ) {
            auto_size = std::to_string(real_size) + "B";
        } else if(real_size >= 1024) {
            auto_size = std::to_string(real_size / 1024) + "KB";
        } else if(real_size >= 1024 * 1024) {
            auto_size = std::to_string(real_size / 1024 / 1024) + "MB";
        } else {
            auto_size = std::to_string(real_size / 1024 / 1024 /1024) + "GB";
        }
        return auto_size;
    }
};