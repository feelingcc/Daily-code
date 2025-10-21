#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>

void* routine(void* args) {
    // 线程分离
    pthread_detach(pthread_self());

    std::string name = static_cast<const char*>(args);
    int cnt = 3;
    while(cnt--) {
        sleep(1);
        std::cout << "I am a new thread , thread name: " << name << " , thread id: " << pthread_self() << std::endl;
    }

    // 线程终止
    pthread_exit(nullptr);
}

int main() {
    
    // 创建线程
    pthread_t tid;
    pthread_create(&tid , nullptr , routine , (void*)"thread - 1");

    int cnt = 3;
    while(cnt--) {
        std::cout << "I am a main thread ,  thread id:" << pthread_self() << std::endl;
        sleep(1);
    }

    // 由于线程分离，pthread_join 函数返回错误码，并且不能获取返回值
    int res = pthread_join(tid , nullptr);
    if(res != 0) {
        std::cout << "pthread_join error res: " << res << std::endl; 
    }

    std::cout << "main thread end" << std::endl;

    return 0;
}