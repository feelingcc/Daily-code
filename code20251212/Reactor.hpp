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
            int timeout = -1;   // 阻塞等待
            while(_is_running) {
                PrintConnection();
                int n = _epoll_model_ptr->waitEvent(_ep_wait_queue , EPOLL_WAIT_SIZE , timeout);
                if(n == -1) { // 等待就绪事件失败
                    break;
                } else if(n == -2) {
                    continue; // 超时
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

        void delConnection(int fd) {
            // 1. epoll 模型中删除
            _epoll_model_ptr->delEvent(fd);

            // 2. 哈希表中删除
            _connect_manager.erase(fd);

            // 3. 关闭文件描述符
            close(fd);

            LogModule::LOG(LogModule::LogLevel::INFO) << "client quit";
        }

        void openWriteReadEvents(int fd , bool write , bool read) {
            // fd不存在
            if(connectionIsExist(fd)) {
                LogModule::LOG(LogModule::LogLevel::INFO) << "fd: " << fd << " is not exists";
                return;
            }

            // 1. 修改哈希表中该fd的事件
            uint32_t new_events = EPOLLET | (write ? EPOLLOUT : 0) | (read ? EPOLLIN : 0);
            _connect_manager[fd]->setEvents(new_events);
            
            // 2. 修改 epoll 模型中fd的事件
            _epoll_model_ptr->modEvent(fd , new_events);
        }

        void stop() {
            _is_running = false;
        }

        void PrintConnection()
        {
            std::cout << "当前Reactor正在进行管理的fd List:";
            for(auto &conn : _connect_manager)
            {
                std::cout << conn.second->getSockfd() << " ";
            }
            std::cout << "\r\n";
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