#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define USAGE_EXIT 1

// ./udpclient server_ip server_port 
int main(int argc , char* argv[]) {

    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        exit(USAGE_EXIT);
    }

    std::string server_ip = argv[1];
    u_int16_t server_port = std::stoi(argv[2]);

    // 1.创建套接字
    int sockfd = socket(AF_INET , SOCK_DGRAM  , 0);
    if(sockfd < 0) {
        std::cerr << "socket create error" << std::endl;
        exit(errno);
    }

    // client不需要显示的bind()
    
    // 填写服务器信息
    struct sockaddr_in dest_addr;
    memset(&dest_addr , 0 , sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(server_port);
    dest_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    while(true) {
        std::cout << "client input: ";
        std::string in;
        std::getline(std::cin , in);
        if(in.empty()) {
            continue;
        }
        // 2.发送消息
        int n = sendto(sockfd , in.c_str() , in.size() , 0 , (sockaddr*)&dest_addr , sizeof(dest_addr));
        (void)n;

        // 占位，这里的server和上面的dest_addr实际是相同的
        struct sockaddr_in server;
        socklen_t len;
        char buffer[1024];
        int m =  recvfrom(sockfd , buffer , sizeof(buffer) - 1 , 0 , (sockaddr*)&server , &len);
        if(m > 0) {
            buffer[m] = 0;
            std::cout << buffer << std::endl;
        }
    }

    return 0;
}