#include "log.h"

int main() {

    // 同步标准输出日志器
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("sync_stdout_logger"); 
    // 异步标准输出日志器
    // std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt<spdlog::async_factory>("async_stdout_logger");
    // 同步文件输出日志器
    // std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("sync_file_logger" , "log.txt");
    // 异步文件输出日志器
    // std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger");

    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S][%-7l] %v");
    logger->debug("{} , {}" , "hello" , "world");
    logger->trace("This is a trace message");
    logger->debug("This is a debug message");
    logger->info("This is an info message");
    logger->warn("This is a warning message");
    logger->error("This is an error message");
    logger->critical("This is a critical message");

    return 0;
}

// int main() {

//     // logger::init_log();

//     // DEBUG_LOG("hello {}" , 1);
//     // INFO_LOG("hello {}" , 2);
//     // WARN_LOG("hello {}" , 3);
//     // ERROR_LOG("hello {}" , 4);
//     // ERROR_LOG("hello");

//     return 0;
// }