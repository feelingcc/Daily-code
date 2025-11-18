#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <filesystem>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include "Mutex.hpp"

namespace LogModule{
    // 定义一个刷新策略的基类
    class FlushStrategy{
        public:
            virtual void synlog(const std::string&) = 0;
            FlushStrategy() = default;
            ~FlushStrategy() = default;
    };

    class ConsoleFlushStrategy : public FlushStrategy{
        public:
            virtual void synlog(const std::string& message) override {
                MutexGuard mg(_mutex);
                std::cout << message << std::endl;
            }
        
            ConsoleFlushStrategy() = default;
            ~ConsoleFlushStrategy() = default;
        private:
            Mutex _mutex;
    };

    const std::string default_path = "./log";
    const std::string default_name = "log.log";
    class FileFlushStrategy : public FlushStrategy{
        public:
            virtual void synlog(const std::string& message) override {
                MutexGuard mg(_mutex);
                std::string pathname = _path + (_path.back() != '/' ? "/" : "") + _name;
                std::ofstream file(pathname , std::ios::app);
                if(!file.is_open()) {
                    std::cerr << "文件打开失败" << std::endl;
                    return;
                }
                file << message << std::endl;
                file.close();
            }
            FileFlushStrategy(const std::string& path = default_path , const std::string& name = default_name)
                :_path(path) , _name(name)
            {
                // 这里主要负责路径不存在创建路径
                if(!std::filesystem::exists(_path)) {
                    try
                    {
                        std::filesystem::create_directory(_path);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
            ~FileFlushStrategy() = default;
        private:
            std::string _path;  // 日志文件所在的路径
            std::string _name;  // 日志文件的名字
            Mutex _mutex;
    };

    // 日志等级
    enum class LogLevel{
        DEBUG , INFO , WARNING , ERROR , FATAL
    };

    std::string LogLevelToString(LogLevel level) {
        switch(level) {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
            case LogLevel::FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    std::string GetTime() {

        // struct tm {
        //     int tm_sec;    /* Seconds (0-60) */
        //     int tm_min;    /* Minutes (0-59) */
        //     int tm_hour;   /* Hours (0-23) */
        //     int tm_mday;   /* Day of the month (1-31) */
        //     int tm_mon;    /* Month (0-11) */
        //     int tm_year;   /* Year - 1900 */
        //     int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
        //     int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
        //     int tm_isdst;  /* Daylight saving time */
        // };
        
        struct tm cur_time;
        time_t timestamp = time(nullptr);
        // 可重入的函数
        localtime_r(&timestamp , &cur_time);
        char format[64];
        snprintf(format , sizeof(format) , "%02d-%02d-%02d %02d:%02d:%02d" , 
            cur_time.tm_year + 1900,
            cur_time.tm_mon + 1,
            cur_time.tm_mday,
            cur_time.tm_hour,
            cur_time.tm_min,
            cur_time.tm_sec
        );
        return format;
    }

    // 日志类
    class Logger{
        public:
            Logger() {
                enableConsoleFlushStrategy();
            }

            void enableConsoleFlushStrategy() {
                // make_unique返回的是一个右值对象，这里调用的是unique_ptr的移动赋值
                _flush_strategy = std::make_unique<ConsoleFlushStrategy>();
            }

            void enableFileFlushStrategy() {
                _flush_strategy = std::make_unique<FileFlushStrategy>();
            }
            
            // 表示一条完整的日志消息
            class LogMessage{
                public:
                    LogMessage(LogLevel level , const std::string& filename , int line , Logger& logger) 
                        :_cur_time(GetTime())
                        ,_log_level(level)
                        ,_pid(getpid())
                        ,_cur_file(filename)
                        ,_line_number(line)
                        ,_logger(logger)
                    {
                        // [2025-10-30 12:24:30][DEBUG][102939][Main.cc][10] - 
                        // 日志的左半部分
                        std::stringstream ss;
                        ss << "[" << _cur_time << "]"
                           << "[" << LogLevelToString(_log_level) << "]"
                           << "[" << _pid << "]"
                           << "[" << _cur_file << "]"
                           << "[" << _line_number << "]"
                           << " - ";
                        
                        message = ss.str();
                    }

                    // 日志的右半部分
                    template<typename T>
                    LogMessage& operator<<(const T& info) {
                        std::stringstream ss;
                        ss << info;
                        message += ss.str();
                        return *this;
                    }

                    ~LogMessage() {
                        // 智能指针不能为空
                        if(_logger._flush_strategy)
                            _logger._flush_strategy->synlog(message);
                    }
                private:
                    std::string _cur_time;
                    LogLevel _log_level;
                    pid_t _pid;
                    std::string _cur_file;
                    int _line_number;
                    std::string message;    // 保存一条完整的日志消息
                    Logger& _logger;
            };  

            // 这里故意返回临时的 LogMessage 对象
            LogMessage operator()(LogLevel level ,const std::string& filename , int line) {
                // 只会调用一次析构函数
                return LogMessage(level , filename , line , *this);
            }
        private:
            std::unique_ptr<FlushStrategy> _flush_strategy;
    };

    // 全局的日志对象
    Logger log;

    // 使用宏，简化用户操作，获取文件名和行号
    #define LOG(le)                           log(le , __FILE__ , __LINE__)
    #define ENABLE_CONSOLE_FLUSH_STRATEGY()   log.enableConsoleFlushStrategy()
    #define ENABLE_FILE_FLUSH_STRATEGY()      log.enableFileFlushStrategy()
}