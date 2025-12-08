#pragma once

#include "Socket.hpp"
#include <memory>
#include <sys/select.h>

class SelectServer{
    const static int fds_size = sizeof(fd_set) * 8;
    const static int default_fd = -1;
    public:
        SelectServer(uint16_t port)
            :_sockptr(std::make_unique<SocketModule::TcpSocket>())
            ,_is_running(false)
        {
            // 创建TCP套接字
            _sockptr->buildTcpListenSocket(port);

            // 初始化 _fds
            for(int i = 0; i < fds_size; i++)
                _fds[i] = default_fd;

            // 添加 listenfd
            _fds[0] = _sockptr->getfd();
        }

        void start() {
            _is_running = true;
            while(_is_running) {
                // rfds 需要每次重置
                fd_set rfds;
                FD_ZERO(&rfds);
                int maxfd = default_fd;
                // 根据辅助数组赋值
                for(int i = 0; i < fds_size; i++) {
                    if(_fds[i] == default_fd)
                        continue;
                    // 设置 rfds 对应比特位
                    FD_SET(_fds[i] , &rfds);
                    // 更新 maxfd
                    maxfd = std::max(maxfd , _fds[i]);
                }
                // 打印 _fds
                printFds();

                // struct timeval timeout = {0 , 0};
                // 阻塞方式
                int n = select(maxfd + 1 , &rfds , nullptr , nullptr , nullptr);
                if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::ERROR) << "select error";
                    break;
                } else if(n == 0) {
                    LogModule::LOG(LogModule::LogLevel::INFO) << "select timeout...";
                    continue;
                } else {
                    // 事件就绪调用事件分发器
                    dispatcher(rfds);
                }
            }
            _is_running = false;
        }

        void accepter() {
            // 获取与客户端的新连接 accept不会被阻塞
            InetAddr client;
            int sockfd = _sockptr->accept(&client);
            if(sockfd < 0) {
                return;
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "get a new link socket: " << sockfd << " - client: " << client.showIpPort();
            
            int i = 0; 
            for(; i < fds_size; i++) {
                if(_fds[i] == default_fd)
                    break;
            }
            // _fds 已满
            if(i == fds_size) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "select fd_set full";
                close(sockfd);
            } else {
                // 添加到辅助数组中
                _fds[i] = sockfd;
            }
        }

        void printFds() {
            std::cout << "_fds[] = ";
            for(int i = 0; i < fds_size; i++) {
                if(_fds[i] != default_fd)
                    std::cout << _fds[i] << " ";
            }
            std::cout << std::endl;
        }

        void recver(int pos) {
            char buffer[4096];
            // 这里读取有bug 读取的可能不是一个完整的报文
            ssize_t n = ::recv(_fds[pos] , buffer , sizeof(buffer) - 1, 0); //recv 不会阻塞
            if(n > 0) {
                buffer[n - 1] = 0;
                std::cout << "client echo: " << buffer << std::endl;
            } else if(n == 0) {
                // client quit
                close(_fds[pos]);
                _fds[pos] = default_fd;
            } else {
                // recv error
                LogModule::LOG(LogModule::LogLevel::ERROR) << "recv error";
                close(_fds[pos]);
                _fds[pos] = default_fd;
            }
        }

        // 事件分发器
        void dispatcher(fd_set& rfds) {
            for(int i = 0; i < fds_size; i++) {
                if(_fds[i] == default_fd)
                    continue;
                // 
                if(FD_ISSET(_fds[i] , &rfds)) {
                    if(_fds[i] == _sockptr->getfd())
                        accepter();
                    else
                        recver(i);
                }
            }
        }
    private:
        std::unique_ptr<SocketModule::Socket> _sockptr;
        bool _is_running;

        // select 辅助数组记录需要监听的文件描述符
        int _fds[fds_size];
};