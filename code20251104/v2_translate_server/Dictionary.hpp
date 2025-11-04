#pragma once

#include "Log.hpp"
#include <unordered_map>
#include <fstream>
#include "InetAddr.hpp"

#define OPEN_FILE_ERRO 1

std::string sep = ": ";
std::string default_file_pathname = "./Dictionary.txt";

// 英文翻译成中文的类
class Dictionary{
    public:
        Dictionary(const std::string& pathname = default_file_pathname) 
            :_file_pathanme(pathname)
        {}
        
        bool loadDictionaryFile() {
            std::ifstream in(_file_pathanme);
            if(!in.is_open()) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "文件打开失败";
                return false;
            }
            std::string line;
            while(std::getline(in , line)) {
                // 1. 解析一行字符串内容 格式 apple: 苹果
                size_t pos = line.find(sep);
                if(pos == std::string::npos) {
                    // 格式错误: 没有找到分隔符
                    LogModule::LOG(LogModule::LogLevel::WARNING) << " [格式错误]";
                    continue;
                }
                std::string english_word = line.substr(0 , pos);
                std::string chinese_word = line.substr(pos + sep.size());
                if(english_word.empty() || chinese_word.empty()) {
                    LogModule::LOG(LogModule::LogLevel::WARNING) << line << " [格式错误]";
                    continue;
                }
                // 2. 添加到哈希表中
                _dict_hash.insert({english_word , chinese_word});
                LogModule::LOG(LogModule::LogLevel::INFO) << line << " [加载成功]";
            }
            in.close();
            return true;
        }

        std::string translate(const std::string& english_word , const InetAddr& addr) {
            std::unordered_map<std::string , std::string>::iterator iter = _dict_hash.find(english_word);
            if(iter == _dict_hash.end()) {
                LogModule::LOG(LogModule::LogLevel::INFO) << "进入翻译模块: "
                                                          << "client ip: "<< addr.getIP() << " "
                                                          << "client port: " << addr.getPort() << " "
                                                          << english_word << " -> " << "None";
                return "None";
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "进入翻译模块: "
                                                      << "client ip: "<< addr.getIP() << " "
                                                      << "client port: " << addr.getPort() << " "
                                                      << english_word << " -> " << iter->second;
            return iter->second;
        }
    private:
        std::string _file_pathanme;
        std::unordered_map<std::string , std::string> _dict_hash;
};