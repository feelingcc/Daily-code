#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace logger{
    #define DEFAULT_TARGET "stdout" // 默认输出目标
    #define FILE_TARGET "file"      // 文件输出目标
    #define DEFAULT_LOG_LEVEL 1     // 默认的日志输出级别
    #define DEFAULT_LOG_PATTERN "[%Y-%m-%d %H:%M:%S][%-7l]%v"  // 默认的日志输出格式
    #define DEFAULT_FILE_LOG "log.txt"  // 默认的输出文件名
    // 定义日志配置
    struct LogSetting{
        bool async = false;                         // 是否开启异步日志
        int level = DEFAULT_LOG_LEVEL;              // 日志等级
        std::string format = DEFAULT_LOG_PATTERN;   // 日志的输出格式
        std::string target = DEFAULT_TARGET;        // 日志的输出目标
        std::string filename = DEFAULT_FILE_LOG;    // 若日志输出目标为文件，则需要指定文件的名称
    };
    // 声明全局的日志器
    extern std::shared_ptr<spdlog::logger> g_logger; 
    // 声明全局的初始化接口
    extern void init_log(const LogSetting& log_setting = LogSetting());
    // 定义日志宏
    #define DEBUG_LOG(format , ...)  logger::g_logger->debug(std::string("[{}:{}]: ") + format , __FILE__ , __LINE__ , ##__VA_ARGS__);
    #define INFO_LOG(format , ...)  logger::g_logger->info(std::string("[{}:{}]: ") + format , __FILE__ , __LINE__ , ##__VA_ARGS__);
    #define WARN_LOG(format , ...)  logger::g_logger->warn(std::string("[{}:{}]: ") + format , __FILE__ , __LINE__ , ##__VA_ARGS__);
    #define ERROR_LOG(format , ...)  logger::g_logger->error(std::string("[{}:{}]: ") + format , __FILE__ , __LINE__ , ##__VA_ARGS__);
}