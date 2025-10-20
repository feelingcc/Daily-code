#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>

int flag = 100;

std::string to_hex(pthread_t tid) {
    char buffer[64];
    snprintf(buffer , sizeof(buffer) , "0x%lx" , tid);
    // std::cout << buffer << std::endl;
    return buffer;
}

void* routine(void* args) {
    int cnt = 5;
    while(cnt--) {
        sleep(1);
        std::cout << "I am a new thread thread_name: " << static_cast<const char*>(args) <<" thread_id: " << to_hex(pthread_self()) << std::endl;
        flag++;
    }

    return reinterpret_cast<void*>(100);
}

int main() {

    pthread_t tid;
    int n = pthread_create(&tid , nullptr, routine , (void*)"thread - 1");

    int cnt = 5;
    while(cnt--) {
        std::cout << "I am a main thread thread_id: " << to_hex(pthread_self())  << " flag = " << flag << std::endl; 
        sleep(1);
    }

    // 线程等待
    void* res = nullptr;
    pthread_join(tid , &res);
    std::cout << "res: " << reinterpret_cast<long long>(res) << std::endl;

    std::cout << to_hex(pthread_self()) << std::endl;
    std::cout << pthread_self() << std::endl;
    printf("%lx" , pthread_self());

    return 0;
}