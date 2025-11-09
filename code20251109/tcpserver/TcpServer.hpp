#pragma once

#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"
#include <signal.h>
#include <pthread.h>

using task_t = std::function<void()>;
using callback_t = std::function<std::string(const std::string&)>; 

const int default_listenfd = -1;
const int default_backlog = 5;

// 服务器禁止拷贝和赋值
class TcpServer : public NoCopy{
    public:
        TcpServer(u_int16_t port , callback_t callback) 
            :_listenfd(default_listenfd)
            ,_port(port)
            ,_is_running(false)
            ,_callback(callback)
        {}

        void init() {
            // signal(SIGCHLD , SIG_IGN);
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
                    // buffer是一个命令
                    buffer[n] = 0;
                    LogModule::LOG(LogModule::LogLevel::INFO) << "server read from " << client.showIpPort() << ": " << buffer;
                    std::string result = "server result: \n";
                    result += _callback(buffer);  // 将数据进行回调处理
                    write(sockfd , result.c_str() , result.size());
                } else if(n == 0) {
                    // 客户端断开连接
                    LogModule::LOG(LogModule::LogLevel::INFO) << "client disconnection";
                    close(sockfd);
                    break;
                } else {
                    // 读取错误
                    LogModule::LOG(LogModule::LogLevel::WARNING) << "read error";
                    close(sockfd);
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
                // version1: 多进程 tcpserver
                /*
                pid_t n = fork();
                if(n == 0) {    // child process
                    // 关闭不需要的文件描述符
                    close(_listenfd); // 监听是主进程做的事情，子进程只需要负责与一个客户端进行通信即可
                    if(fork() > 0)  // child process 直接退出   
                        exit(OK);
                    // child child process 孙子进程被系统领养并回收
                    server(sockfd , client_addr);
                    exit(OK);
                } else if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "fork error";
                    exit(FORK_ERRO);
                }
                // parent process 不能阻塞的等，否则退化为单进程服务器
                // 1. 使用信号 signal(SIGCHLD , SIG_IGN);
                // 2. 子进程再创建子进程的方式执行server，之后子进程退出，孙子进程被1号系统进程领养并回收
                close(sockfd);  // 这里会将子进程文件描述表中指向的struct file的引用计数-1
                */

                // version2: 多线程 tcpserver
                pthread_t tid;
                ThreadDate* td = new ThreadDate(sockfd , client_addr , this);
                int n = pthread_create(&tid , nullptr , threadRoutine , td);

                // version3: 引入线程池
                // ThreadPool<task_t>::getInstance()->enqueue([this , sockfd , client_addr](){
                //     this->server(sockfd , client_addr);
                // });
            }
            _is_running= false;
        }
    private:
        struct ThreadDate{
            ThreadDate(int fd , const InetAddr& addr , TcpServer* tcptr)
                :sockfd(fd)
                ,client_addr(addr)
                ,tsver(tcptr)
            {}
            int sockfd;
            InetAddr client_addr;
            TcpServer* tsver;
        };

        static void* threadRoutine(void* args) {
            pthread_detach(pthread_self());
            ThreadDate* td = static_cast<ThreadDate*>(args);
            td->tsver->server(td->sockfd , td->client_addr);
            delete td;
            return nullptr;
        }

    private:
        int _listenfd;
        // std::string _ip;
        u_int16_t _port;
        bool _is_running;
        callback_t _callback;
};