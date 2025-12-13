#pragma once

#include "Connection.hpp"

class Channel : public Connection{
    public:
        Channel(int sockfd  , const InetAddr& client)
            :_sockfd(sockfd)
            ,_client_addr(client)
        {
            // 1. 设置 Channel 监听的事件和开启 ET 模式
            setEvents(EPOLLIN | EPOLLET); // todo
            // 2. 设置文件描述符为非阻塞
            setNonBlock(_sockfd);
        }

        virtual int getSockfd() override { return _sockfd; }

        virtual void recver() override {
            
        }

        virtual void sender() override {

        }

        virtual void expect() override {

        }
    private:    
        int _sockfd;
        std::string _inbuffer;
        std::string _outbuffer;
        InetAddr _client_addr;
};