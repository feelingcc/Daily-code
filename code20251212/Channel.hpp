#pragma once

#include "Connection.hpp"

#define RECV_SIZE 4096

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
            char buffer[RECV_SIZE];
            while(true) {
                buffer[0] = 0;  // 清空字符串
                ssize_t n = recv(_sockfd , buffer , RECV_SIZE - 1 , 0); // 非阻塞方式读取
                if(n > 0) {
                    buffer[n] = 0;
                    _inbuffer += buffer;
                } else if(n == 0) {
                    // 客户端关闭 异常处理
                    except();
                    return;
                } else {
                    if(errno == EAGAIN) {
                        // 非阻塞读取完毕
                        break;
                    } else if(errno == EINTR) {
                        // 读取被信号打断
                        continue;
                    } else {
                        // recv error
                        LogModule::LOG(LogModule::LogLevel::ERROR) << "channel recv error";
                        except();
                        return;
                    }
                }
            }
            // ET 非阻塞读取数据完毕
            // 1. 判断是否报文是否是一个完整的报文，如果是多个报文？数据不完整问题或TCP粘包问题

        }

        virtual void sender() override {

        }

        virtual void except() override {

        }
    private:    
        int _sockfd;
        std::string _inbuffer;
        std::string _outbuffer;
        InetAddr _client_addr;
};