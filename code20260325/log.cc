#include "log.h"

namespace logger{
    // 定义全局的日志器
    std::shared_ptr<spdlog::logger> g_logger; 
    // 定义全局的初始化日志器接口
    void init_log(const LogSetting& log_setting) {
        // 初始化全局日志器，是同步/异步，目标是标准输出/文件
        if(log_setting.async) {
            if(log_setting.target == DEFAULT_TARGET) 
                g_logger = spdlog::stdout_color_mt<spdlog::async_factory>("async_stdout_logger");
            else 
                g_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger" , log_setting.filename);
        } else {
            if(log_setting.target == DEFAULT_TARGET) 
                g_logger = spdlog::stdout_color_mt("sync_stdout_logger");
            else 
                g_logger = spdlog::basic_logger_mt("sync_file_logger" , log_setting.filename);
        }
        // 设置全局日志器的日志级别
        g_logger->set_level((spdlog::level::level_enum)log_setting.level); // 1:debug 2:info 3:warn 4:error off:6
        // 设置全局日志器的日志格式
        g_logger->set_pattern(log_setting.format);  // [%Y-%m-%d %H:%M:%S][%-7l]%v
    }
}
