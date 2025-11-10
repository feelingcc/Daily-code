#pragma once

#include "Socket.hpp"

using namespace SocketMoudle;

class TcpServer{
    public:
        TcpServer(u_int16_t port)
            :_tcp_listensocket(std::make_unique<TcpSocket>(port))
            ,_port(port)
            ,_is_running(false)
        {
            // 多态调用
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
            }
            _is_running = false;    
        }
    private:
        std::unique_ptr<Socket> _tcp_listensocket;
        u_int16_t _port;
        bool _is_running;
};