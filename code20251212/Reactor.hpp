#pragma once

#include <iostream>
#include <unordered_map>
#include "Epoller.hpp"
#include "Connection.hpp"

#define EPOLL_WAIT_SIZE 1024

class Reactor{
    bool connectionIsExist(int key_fd) {
        auto iter = _connect_manager.find(key_fd);
        return iter == _connect_manager.end();
    }

    void dispatcher(int n) {
        for(int i = 0; i < n; i++) {
            int ready_fd = _ep_wait_queue[i].data.fd;
            uint32_t ready_event = _ep_wait_queue[i].events;
            
            // 就绪事件发生错误或客户端断开链接，交到下层I/O事件中处理异常
            if(ready_event & EPOLLERR) {
                ready_event |= (EPOLLIN | EPOLLOUT);
            }
            if(ready_event & EPOLLHUP) {
                ready_event |= (EPOLLIN | EPOLLOUT);
            }

            // 读事件就绪
            if(ready_event & EPOLLIN) {
                // 为了提高代码的鲁棒性，发生错误时，只处理一次，避免处理多次，需判断链接Connection是否存在
                if(!connectionIsExist(ready_fd)) {
                    // 多态调用 Conncetion 的读方法 Connction 可能是 Listener 或 Channel
                    _connect_manager[ready_fd]->recver();
                }
            }

            // 写事件就绪
            if(ready_event & EPOLLOUT) {
                // 为了提高代码的鲁棒性，发生错误时，只处理一次，需判断链接Connection是否存在
                if(!connectionIsExist(ready_fd)) {
                    // 多态调用 Conncetion 的写方法 Connction 可能是 Listener 或 Channel
                    _connect_manager[ready_fd]->sender();
                }
            }
        }
    }
    public:
        Reactor()
            :_epoll_model_ptr(std::make_unique<Epoller>())
            ,_is_running(false)
        {}

        void start() {
            // 增加代码的鲁棒性，若 _connect_manager 为空则不启动服务器
            if(_connect_manager.empty()) {
                return;
            }

            _is_running = true;
            int timeout = 1000;
            while(_is_running) {
                int n = _epoll_model_ptr->waitEvent(_ep_wait_queue , EPOLL_WAIT_SIZE , timeout);
                if(n < 0) { // 等待就绪事件失败
                    break;
                }
                // 就绪事件分发器
                dispatcher(n);
            }
            _is_running = false;    
        }

        void addConnection(std::shared_ptr<Connection> conn) {
            // 1. 判断 connect_manager 是否已经存在 conn
            std::unordered_map<int , std::shared_ptr<Connection>>::iterator iter = _connect_manager.find(conn->getSockfd());
            if(iter != _connect_manager.end()) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "connection have existed";
                return;
            }

            // 2. 获取 Connection 的  fd 与 events
            int fd = conn->getSockfd();
            uint16_t events = conn->getEvents();

            // 3. 将 fd 与 events 添加到 epoller 模型中
            _epoll_model_ptr->addEvent(fd , events);

            // 4. 设置 conn 的回指指针
            conn->setReactorPtr(this);

            // 5. 将 fd 与 conn 添加到 connect_manager 管理容器中
            _connect_manager[fd] = conn;
        }

        void stop() {
            _is_running = false;
        }
    private:
        // 1.epoll 模型
        std::unique_ptr<Epoller> _epoll_model_ptr;
        // 2.epoll 用户就绪队列
        struct epoll_event _ep_wait_queue[EPOLL_WAIT_SIZE];
        // 3.链接管理器 [fd : Connection]
        std::unordered_map<int , std::shared_ptr<Connection>> _connect_manager;
        // 4.是否运行标志
        bool _is_running;
};