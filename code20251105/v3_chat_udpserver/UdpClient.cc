#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

#define USAGE_EXIT 1

std::string server_ip;
u_int16_t server_port = 0;

/*
*   客户端需要两个线程:
*       1. 一个线程负责发消息
*       2. 一个线程负责收消息
*   如果只有一个线程，可能已经收到了消息，但是被发消息的cin阻塞，导致看不到消息
*/  

void* sendHandler(void* args) {
    int sockfd = reinterpret_cast<long long>(args);
    // 填写服务器信息
    struct sockaddr_in dest_addr;
    memset(&dest_addr , 0 , sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(server_port);
    dest_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    while(true) {
        std::cout << "client input: ";      // 向文件描述1标准输出打印，从而使发送消息和收消息打印消息分离（分离到不同的Shell中）
        std::string in;
        std::getline(std::cin , in);
        if(in.empty()) {
            continue;
        }
        // 2.发送消息
        int n = sendto(sockfd , in.c_str() , in.size() , 0 , (sockaddr*)&dest_addr , sizeof(dest_addr));
        (void)n;
        // debug: 退出消息是否能被其他用户获取到
        // sleep(1);
        // 3.用户退出
        if(in == "quit") {
            break;  // 发送线程结束
        }
    }
    return nullptr;
}

void* recvHandler(void* args) {
    int sockfd = reinterpret_cast<long long>(args);
    while(true) {
        // 占位，这里的 server 和sendHandler中的 dest_addr 实际是相同的
        struct sockaddr_in server;
        socklen_t len;
        char buffer[1024];
        int n =  recvfrom(sockfd , buffer , sizeof(buffer) - 1 , 0 , (sockaddr*)&server , &len);
        if(n > 0) {
            buffer[n] = 0;
            std::cerr << buffer << std::endl;   // 向文件描述2标准错误打印
        }
    }
    return nullptr;
}

// ./udpclient server_ip server_port 
int main(int argc , char* argv[]) {

    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        exit(USAGE_EXIT);
    }

    server_ip = argv[1];
    server_port = std::stoi(argv[2]);

    // 1.创建套接字
    int sockfd = socket(AF_INET , SOCK_DGRAM  , 0);
    if(sockfd < 0) {
        std::cerr << "socket create error" << std::endl;
        exit(errno);
    }
    // client不需要显示的bind()

    // 0: sender 1: recver
    pthread_t tids[2];
    pthread_create(&tids[0] , nullptr , sendHandler , reinterpret_cast<void*>(sockfd));
    pthread_create(&tids[1] , nullptr , recvHandler , reinterpret_cast<void*>(sockfd));

    pthread_join(tids[0] , nullptr);
    // 发送消息线程结束
    // 取消收消息线程
    pthread_cancel(tids[1]);
    pthread_join(tids[1] , nullptr);
    std::cout << "send thread recv thread and main thread end" << std::endl;

    return 0;
}