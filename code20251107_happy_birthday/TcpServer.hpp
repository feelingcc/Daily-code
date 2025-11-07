#pragma once

#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

const int default_listenfd = -1;
const int default_backlog = 5;

// 服务器禁止拷贝和赋值
class TcpServer : public NoCopy{
    public:
        TcpServer(u_int16_t port) 
            :_listenfd(default_listenfd)
            ,_port(port)
            ,_is_running(false)
        {}

        void init() {
            // 1.创建网络文件描述符
            _listenfd = socket(AF_INET , SOCK_STREAM , 0);
            if(_listenfd < 0) {
                LogModule::LOG(LogModule::LogLevel::FATAL) << "create socket error";
                exit(SOCKET_ERROR);
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "create socket success - listenfd: " << _listenfd;
            // 2.构造服务器信息并绑定
            InetAddr addr(_port);
            int n = bind(_listenfd , addr.getSockaddr() , addr.getSockaddrLen());
            if(n < 0) {
                LogModule::LOG(LogModule::LogLevel::FATAL) << "bind error";
                exit(BIND_ERROR);
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "bind success - listenfd: " << _listenfd;
            // 3.监听网络文件描述符
            n = listen(_listenfd  , default_backlog);
            if(n < 0) {
                LogModule::LOG(LogModule::LogLevel::FATAL) << "listen error";
                exit(LISTEN_ERROR);
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "listen success - listenfd: " << _listenfd;
        }

        void server(int sockfd , const InetAddr& client) {
            while(true) {
                char buffer[1024];
                ssize_t n = read(sockfd , buffer , sizeof(buffer) - 1);
                if(n > 0) {
                    buffer[n] = 0;
                    LogModule::LOG(LogModule::LogLevel::INFO) << "server read from " << client.showIpPort() << ": " << buffer;
                    std::string echo_message = "server echo: ";
                    echo_message += buffer;
                    write(sockfd , echo_message.c_str() , echo_message.size());
                } else if(n == 0) {
                    // 客户端断开连接
                    LogModule::LOG(LogModule::LogLevel::INFO) << "client disconnection";
                    break;
                } else {
                    // 读取错误
                    LogModule::LOG(LogModule::LogLevel::WARNING) << "read error";
                    break;
                }
            }
        }

        void start() {
            _is_running = true;
            while(_is_running) {
                struct sockaddr_in client;
                socklen_t addrlen = sizeof(client);
                // 4.与客户端建立连接并通信的文件描述符
                int sockfd = accept(_listenfd , (struct sockaddr*)&client , &addrlen);
                if(sockfd < 0) {
                    continue;   // 与客户端建立连接失败应继续与其他客户端建立
                }
                LogModule::LOG(LogModule::LogLevel::INFO) << "establish connection with client - sockfd: " << sockfd;
                InetAddr client_addr(client);
                server(sockfd , client_addr);
            }
            _is_running= false;
        }
    private:
        int _listenfd;
        // std::string _ip;
        u_int16_t _port;
        bool _is_running;
};