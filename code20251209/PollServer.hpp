#pragma once

#include "Socket.hpp"
#include <memory>
#include <poll.h>

class PollServer{
    const static int default_fd = -1;
    const static int default_capacity = 1;
    public:
        PollServer(uint16_t port)
            :_sockptr(std::make_unique<SocketModule::TcpSocket>())
            ,_is_running(false)
            ,_capacity(default_capacity)
        {
            // 创建TCP套接字
            _sockptr->buildTcpListenSocket(port);

            // 初始化 _fds
            _fds = new struct pollfd[16];
            for(int i = 0; i < _capacity; i++)
                _fds[i] = {default_fd , 0 , 0};

            // 监听 listenfd 读事件
            _fds[0] = {_sockptr->getfd() , POLLIN , 0};
        }

        void start() {
            _is_running = true;
            while(_is_running) {
                
                // 打印 _fds
                printFds();

                // 阻塞方式
                int n = poll(_fds , _capacity , -1);
                if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::ERROR) << "poll error";
                    break;
                } else if(n == 0) {
                    LogModule::LOG(LogModule::LogLevel::INFO) << "poll timeout...";
                    continue;
                } else {
                    // 事件就绪调用事件分发器
                    dispatcher();
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
            for(; i < _capacity; i++) {
                if(_fds[i].fd == default_fd)
                    break;
            }
            // _fds 已满
            if(i == _capacity) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "poll fds full";
                // 扩容
                struct pollfd* temp = new struct pollfd[_capacity * 2];
                for(int i = 0; i < _capacity; i++) {
                    temp[i] = _fds[i];
                }
                for(int i = _capacity; i < _capacity * 2; i++) {
                    temp[i] = {default_fd , 0 , 0};
                }
                delete[] _fds;
                _fds = temp;
                _capacity *= 2;
            } 
            LogModule::LOG(LogModule::LogLevel::DEBUG) << "poll relloc capaciyt: " << _capacity;
            // 添加到辅助数组中·
            _fds[i].fd = sockfd;
            _fds[i].events = POLLIN;
            _fds[i].revents = 0;
        }

        void printFds() {
            std::cout << "_fds[] = ";
            for(int i = 0; i < _capacity; i++) {
                if(_fds[i].fd != default_fd)
                    std::cout << _fds[i].fd << " ";
            }
            std::cout << std::endl;
        }

        void recver(int pos) {
            char buffer[4096];
            // 这里读取有bug 读取的可能不是一个完整的报文
            ssize_t n = ::recv(_fds[pos].fd , buffer , sizeof(buffer) - 1, 0); //recv 不会阻塞
            if(n > 0) {
                buffer[n - 1] = 0;
                std::cout << "client echo: " << buffer << std::endl;
            } else if(n == 0) {
                // client quit
                close(_fds[pos].fd);
                _fds[pos].fd = default_fd;
                _fds[pos].events = 0;
                _fds[pos].revents = 0;
            } else {
                // recv error
                LogModule::LOG(LogModule::LogLevel::ERROR) << "recv error";
                close(_fds[pos].fd);
                _fds[pos].fd = default_fd;
                _fds[pos].events = 0;
                _fds[pos].revents = 0;
            }
        }

        // 事件分发器
        void dispatcher() {
            for(int i = 0; i < _capacity; i++) {
                if(_fds[i].fd == default_fd)
                    continue;
                // 该文件描述符的读事件就绪
                if(_fds[i].revents & POLLIN) {
                    if(_fds[i].fd == _sockptr->getfd())
                        accepter(); // 获取新连接
                    else
                        recver(i);  // 某个连接读事件就绪
                }
            }
        }
    private:
        std::unique_ptr<SocketModule::Socket> _sockptr;
        bool _is_running;

        // poll 结构体数组 可以通过 new 来动态扩容
        struct pollfd* _fds;
        unsigned int _capacity;
};