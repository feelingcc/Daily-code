#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Log.hpp"

class Utils{
    public:
        // 读取文件所有内容的函数
        static bool ReadFileContent(const std::string& pathname , std::string* res) {
            std::ifstream in(pathname , std::ios::in);
            if(!in.is_open()) {
                LogModule::LOG(LogModule::LogLevel::ERROR) << "open file error";
                return false;
            }
            std::string line;
            while(std::getline(in , line)) {
                *res += line;
            }
            in.close();
            return true;
        }

        // 读取字符串中的第一行内容
        static bool ReadOnlyLineStr(std::string& buffer , std::string* str , const std::string sep = "\n") {
            size_t pos = buffer.find(sep);
            if(pos == std::string::npos) {
                return false;
            }
            *str = buffer.substr(0 , pos);
            buffer.erase(0 , pos + sep.size());
            return true;
        }
};