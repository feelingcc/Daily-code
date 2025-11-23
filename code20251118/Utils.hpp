#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Log.hpp"

class Utils{
    public:
        // 读取文件所有内容的函数
        static bool ReadFileContent(const std::string& pathname , std::string* res) {
            res->clear();
            // std::ifstream in(pathname , std::ios::in);
            // // LogModule::LOG(LogModule::LogLevel::DEBUG) << pathname;
            // if(!in.is_open()) {
            //     LogModule::LOG(LogModule::LogLevel::ERROR) << "open file error";
            //     return false;
            // }
            // std::string line;
            // while(std::getline(in , line)) {
            //     *res += line;
            // }
            // // std::cout << "res = " << *res;
            // in.close();

            //  可能获取的是图片、网页等，所以需要二进制的方式读取
            std::ifstream in(pathname , std::ios::binary);
            if(!in.is_open()) {
                LogModule::LOG(LogModule::LogLevel::ERROR) << "open file error";
                return false;
            }
            char buffer[500000];
            in.read(buffer , sizeof(buffer));
            // *res = buffer;
            // 赋值到string中若是图片二进制数据可能遇到\0结束，所以指定大小创建
            *res = std::string(buffer , getFileContentSize(pathname));
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

        // 获取文件大小
        static size_t getFileContentSize(const std::string& pathname) {
            std::ifstream in(pathname , std::ios::binary);
            if(!in.is_open()){
                LogModule::LOG(LogModule::LogLevel::ERROR) << "open file error";
                return false;
            }
            in.seekg(0 , std::ios::end);
            size_t res = in.tellg();    // 获取当前文件指针位置
            in.seekg(0 , std::ios::beg);
            in.close();
            return res;
        }
};