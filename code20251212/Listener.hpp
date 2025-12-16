#pragma once

#include <memory>
#include "Socket.hpp"
#include "Channel.hpp"
#include "Reactor.hpp"

class Listener : public Connection{
    const static uint16_t default_port = 8080;
    public:
        Listener(uint16_t port = default_port)
            :_port(port) 
            ,_socket_ptr(std::make_unique<SocketModule::TcpSocket>())
        {   
            // 1. 创建TCP套接字
            _socket_ptr->buildTcpListenSocket(_port); 
            // 2. 设置监听的事件 listener 只负责获取新连接 | 开启 ET 模式
            setEvents(EPOLLIN | EPOLLET);  
            // 3. 设置文件描述符为非阻塞模式
            setNonBlock(_socket_ptr->getfd());
        }

        virtual void recver() override {
            InetAddr client;
            // ET 模式 必须一次性读完所有客户端链接
            while(true) {
                int sockfd = _socket_ptr->accept(&client);
                if(sockfd < 0) {
                    // 本次读取完毕
                    if(errno == EAGAIN) {
                        break;
                    } else if(errno == EINTR) {
                        // accept 被信号打断
                        continue;
                    } else {
                        // 读取错误
                        break;
                    }
                }
                // 读取成功 sockfd > 0
                // 1. 创建 Channel 对象
                std::shared_ptr<Connection> conn = std::make_shared<Channel>(sockfd , client);
                // 2. 通过回指指针将 Channel 添加到 Reactor 链接管理容器中
                getReactorPtr()->addConnection(conn);
            }
        }

        virtual void sender() override {}
        virtual void except() override {}
        virtual int getSockfd() override {
            return _socket_ptr->getfd();
        }
    private:
        uint16_t _port;
        std::unique_ptr<SocketModule::Socket> _socket_ptr;  // socket中包含fd
};
