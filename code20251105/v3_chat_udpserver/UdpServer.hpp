#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <functional>
#include "Log.hpp"
#include "InetAddr.hpp"

using method_t = std::function<void(int , const std::string ,const InetAddr&)>;

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
            local.sin_port = htons(_port);
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
                if(n > 0) {
                    // 接收到某个用户发送的消息
                    buffer[n] = 0;
            
                    // 将该消息交给消息转发类处理
                    _handler_data(_sockfd , buffer ,InetAddr(client));
                    
                    //sendto(_sockfd , result.c_str() , result.size() , 0 , (sockaddr*)&client , client_addrlen);
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

