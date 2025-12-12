#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include "Common.hpp"
#include "Epoller.hpp"
#include "Connection.hpp"

class Reactor{
    public:
        Reactor()
            :_epoll_model_ptr(std::make_unique<Epoller>())
            ,_is_running(false)
        {}

        void start() {
            _is_running = true;
            while(_is_running) {

            }
            _is_running = false;    
        }

        void addConnection(std::shared_ptr<Connection> conn) {
            // 1. 判断 connect_manager 是否已经存在 conn

            // 2. 获取 Connection 的  fd 与 events
            int fd = conn->getSockfd();
            uint16_t events = conn->getEvents();

            // 3. 将 fd 与 events 添加到 epoller 模型中

            // 4. 将 fd 与 conn 添加到 connect_manager 管理容器中
        }

        void stop() {
            _is_running = false;
        }
    private:
        // 1.epoll 模型
        std::unique_ptr<Epoller> _epoll_model_ptr;
        // 2.链接管理器 [fd : Connection]
        std::unordered_map<int , std::shared_ptr<Connection>> _connect_manager;
        // 3.是否运行标志
        bool _is_running;
};