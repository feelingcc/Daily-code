#pragma once

#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"

namespace SocketMoudle{
    const static int default_backlog = 16;
    // 设计为虚基类，TCP和UDP的模板类
    class Socket{
        public:
            virtual ~Socket() {}
            virtual void socket() = 0;
            virtual void bind(uint16_t server_port) = 0;
            virtual void listen(int backlog) = 0;
            virtual std::shared_ptr<Socket> accept(InetAddr* client_addr) = 0;
            virtual void close() = 0;
            virtual int recv(std::string* res) = 0;
            virtual int send(const std::string& message) = 0;
            virtual void connect(const InetAddr& server_addr) = 0;
        public:
            void buildTcpListenSocket(uint16_t server_port , int backlog = default_backlog) {
                socket();
                bind(server_port);
                listen(default_backlog);
            }
            void buildTcpClientListendSocket() {
                socket();
            }
    };

    const static int default_sockfd = -1;
    class TcpSocket : public Socket{
        public: 
            TcpSocket() :_sockfd(default_sockfd) {}         // 初始化sockfd为listenfd
            TcpSocket(int acceptfd) :_sockfd(acceptfd) {}   // 初始化sockfd为accpetfd

            virtual void socket() override {
                _sockfd = ::socket(AF_INET , SOCK_STREAM , 0);
                if(_sockfd < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "socket create error";
                    exit(SOCKET_ERROR);
                }
                LogModule::LOG(LogModule::LogLevel::INFO) << "socket create success - sockfd: " << _sockfd;
            }

            virtual void bind(uint16_t server_port) override {
                InetAddr server_addr(server_port);
                int n = ::bind(_sockfd , server_addr.getSockaddr() , server_addr.getSockaddrLen());
                if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "bind error";
                    exit(BIND_ERROR);
                }
                LogModule::LOG(LogModule::LogLevel::INFO) << "bind success - sockfd: " << _sockfd;
            }

            virtual void listen(int backlog) override {
                int n = ::listen(_sockfd , backlog);
                if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "listen error";
                    exit(LISTEN_ERROR);
                }
                LogModule::LOG(LogModule::LogLevel::INFO) << "listen success - sockfd: " << _sockfd;
            }

            virtual std::shared_ptr<Socket> accept(InetAddr* client_addr) override {
                struct sockaddr_in client;
                socklen_t addrlen = sizeof(client);
                int acceptfd = ::accept(_sockfd , (struct sockaddr*)&client , &addrlen);
                if(acceptfd < 0) {
                    LogModule::LOG(LogModule::LogLevel::WARNING) << "accpet warning";
                    return nullptr;
                }
                client_addr->setSockaddrIn(client); // 输出型参数
                return std::make_shared<TcpSocket>(acceptfd);   // 多个服务器可以共享一个客户端的链接
            }

            virtual void close() override {
                if(_sockfd > 0)
                    ::close(_sockfd);   // close(listenfd) 或 close(accpetfd)
            }

            // recv返回值和::recv返回值保持一致
            virtual int recv(std::string* res) override {
                char buffer[1024];
                ssize_t n = ::recv(_sockfd , buffer , sizeof(buffer) , 0);
                *res += buffer; // 读取可能会不完整
                return n;
            }

            virtual int send(const std::string& message) override {
                ssize_t n = ::send(_sockfd , message.c_str() , message.size() , 0);
                return n;
            }

            virtual void connect(const InetAddr& server_addr) {
                int n = ::connect(_sockfd , server_addr.getSockaddr() , server_addr.getSockaddrLen());
                if(n < 0) {
                    LogModule::LOG(LogModule::LogLevel::FATAL) << "connect server error";
                    exit(CONNECT_ERROR);
                }
            }
        private:
            int _sockfd;    // listenfd 或 accpetfd 
    };
}