#include "Log.hpp"
#include "ThreadPool.hpp"

using namespace LogModule;

struct Task{
    void operator()(){
        std::cout << "正在处理一个任务..." << std::endl;
    }
};


int main() {

    ENABLE_CONSOLE_FLUSH_STRATEGY();
    ThreadPool<Task>::getInstance()->start();

    // 单生产多消费
    int cnt = 10;
    while(cnt--) {
        ThreadPool<Task>::getInstance()->enqueue(Task());
        sleep(1);
    }

    ThreadPool<Task>::getInstance()->stop();
    ThreadPool<Task>::getInstance()->join();

    // LOG(LogLevel::DEBUG) << "hello world";

    // Logger log;
    // log.enableFileFlushStrategy();
    // log(LogLevel::DEBUG , __FILE__ , __LINE__) << "hello world";
    // log(LogLevel::INFO , __FILE__ , __LINE__) << "hello world";
    // log(LogLevel::WARNING , __FILE__ , __LINE__) << "hello world";
    // log(LogLevel::ERROR, __FILE__ , __LINE__) << "hello world";
    // log(LogLevel::FATAL, __FILE__ , __LINE__) << "hello world";

    return 0;
}