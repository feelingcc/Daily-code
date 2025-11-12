#pragma once

#include "Socket.hpp"
#include <sys/types.h>
#include <sys/wait.h>

using namespace SocketMoudle;

using callback_t = std::function<void(std::shared_ptr<Socket>& , const InetAddr&)>;

class TcpServer{
    public:
        TcpServer(u_int16_t port , callback_t callback)
            :_tcp_listensocket(std::make_unique<TcpSocket>(port))
            ,_port(port)
            ,_is_running(false)
            ,_callback(callback)
        {
            // 多态调用socket/bind/listen
            _tcp_listensocket->buildTcpListenSocket(_port);
        }

        void start() {
            _is_running = true;
            while(_is_running) {
                // 获取客户端的连接
                InetAddr client_addr;
                std::shared_ptr<Socket> tcp_acceptsocket = _tcp_listensocket->accept(&client_addr);
                if(tcp_acceptsocket == nullptr) {
                    continue;
                }
                // 多进程/多线程处理客户端连接
                pid_t pid = fork();
                if(pid < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "server fork error";
                    exit(FORK_ERRO);
                } else if(pid == 0) {
                    // child process
                    // 子进程关闭不需要的文件描述符
                    _tcp_listensocket->close(); // close(accpetfd)， 这里子进程可能会写时拷贝
                    if(fork() > 0)
                        exit(OK);   //  子进程正常退出，父进程回收
                    // 孙子进程被系统领养，自动回收资源
                    // 回调函数处理服务器与客户端的连接
                    _callback(tcp_acceptsocket , client_addr); 
                    tcp_acceptsocket->close();
                    exit(OK);
                }
                // parent process
                // 父进程关闭不需要的文件描述符
                tcp_acceptsocket->close();  // close(listenfd)
                // 回收子进程，循环继续执行获取与客户端的连接
                waitpid(pid , nullptr , 0);
            }
            _is_running = false;    
        }
    private:
        std::unique_ptr<Socket> _tcp_listensocket;
        u_int16_t _port;
        bool _is_running;
        callback_t _callback;
};