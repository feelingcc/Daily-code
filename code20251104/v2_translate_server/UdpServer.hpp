#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <functional>
#include "Log.hpp"
#include "InetAddr.hpp"

using method_t = std::function<std::string(const std::string& , const InetAddr&)>;

using namespace LogModule;

int default_sockfd = -1;

class UdpServer{
    public:
        UdpServer(u_int16_t port , method_t handler) 
            :_sockfd(default_sockfd)
            ,_port(port)
            ,_is_running(false)
            ,_handler_data(handler)
        {}

        void init() {
            _sockfd = socket(AF_INET , SOCK_DGRAM , 0);
            if(_sockfd < 0) {
                LOG(LogLevel::FATAL) << "socket create error";
                exit(errno);
            }
            LOG(LogLevel::INFO) << "socket create success - sockfd: " << _sockfd;

            struct sockaddr_in local;
            bzero(&local , sizeof(local));
            local.sin_family = AF_INET;
            // 而端口和IP需要发送到网络
            local.sin_port = htons(_port);
            // 1.IP需要将点分十进制的字符串转换为4字节整数
            // 2.再将4字节整数转换为网络序列(大端字节序)
            // inet_addr函数将以上2步全部做了
            // local.sin_addr.s_addr = inet_addr(_ip.c_str());
            // 服务端不绑定特定的IP，使用 0.0.0.0 任意IP绑定
            local.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
            int n = bind(_sockfd , (sockaddr*)&local , sizeof(local));
            if(n < 0) {
                LOG(LogLevel::FATAL) << "bind error";
                exit(errno);
            }
            LOG(LogLevel::INFO) << "bind success - sockfd: " << _sockfd;
        }

        void start() {
            _is_running = true;
            while(_is_running) {
                char buffer[1024];
                struct sockaddr_in client;
                socklen_t client_addrlen = sizeof(client);
                ssize_t n = recvfrom(_sockfd , buffer , sizeof(buffer) - 1 , 0 , (sockaddr*)&client , &client_addrlen);
                // n实际接收的字节数
                if(n > 0) {
                    // 解析客户端的 sockaddr_in，由于是网络序列需要转换为本地字节序列
                    // u_int16_t client_port = ntohs(client.sin_port);
                    // std::string client_ip = inet_ntoa(client.sin_addr); // 4字节的网络序列ip -> 字符串风格的本地序列ip

                    // 输入的是一个英文单词
                    buffer[n] = 0;
                    // LOG(LogLevel::DEBUG) << "client_ip: " << client_ip << " "
                    //                      << "client_port: " << client_port << " "
                    //                      << "server recvfrom buffer: " << buffer;

                    // "server echo: " + buffer 报错: const char* + char* 指针 + 指针非法的
                    // std::string echo = "server echo: ";
                    // echo += buffer;

                    // 使用回调函数处理数据
                    std::string result = _handler_data(buffer , InetAddr(client));
                    
                    sendto(_sockfd , result.c_str() , result.size() , 0 , (sockaddr*)&client , client_addrlen);
                }

            }
        }
    private:
        int _sockfd;
        // std::string _ip;
        u_int16_t _port;
        bool _is_running;
        method_t _handler_data;
};

