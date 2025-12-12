#pragma once

#include <iostream>
#include <memory>
#include "Socket.hpp"

class Reactor;

class Connection{
    public:
        // get/set方法
        void setReactorPtr(Reactor* reactor) { _reactor = reactor; }
        Reactor* getReactorPtr() { return _reactor; }
        void setEvents(uint32_t events) { _events = events; }
        uint32_t getEvents() { return _events; }

        // 纯虚方法
        virtual int getSockfd() = 0;
    private:    
        // 1.回指指针
        Reactor* _reactor;
        // 2.监听的事件
        uint32_t _events;
};

class Listener : public Connection{
    const static uint16_t default_port = 8080;
    public:
        Listener(uint16_t port = default_port)
            :_port(port) 
            ,_socket_ptr(std::make_unique<SocketModule::TcpSocket>())
        {   
            // 1.创建TCP套接字
            _socket_ptr->buildTcpListenSocket(_port);   

        }

        virtual int getSockfd() override {
            return _socket_ptr->getfd();
        }
    private:
        uint16_t _port;
        std::unique_ptr<SocketModule::Socket> _socket_ptr;  // socket中包含fd
};

class Channel : public Connection{
    public:
        virtual int getSockfd() override {}
    private:    

};