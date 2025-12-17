#pragma once

#include "Connection.hpp"
#include "Reactor.hpp"

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
            LogModule::LOG(LogModule::LogLevel::DEBUG) << "Channel inbuffer: " << _inbuffer;
            // ET 非阻塞读取数据完毕
            // 1. 判断是否报文是否是一个完整的报文，如果是多个报文？数据不完整问题或TCP粘包问题
            if(!_inbuffer.empty()) {
                // 将数据交付给 Protocol 层处理
                _outbuffer += _callback(_inbuffer);
            }

            // 2. 不为空发送
            if(!_outbuffer.empty()) {
                sender();
            }
        }

        virtual void sender() override {
            while(true) {
                ssize_t n = send(_sockfd , _outbuffer.c_str() , _outbuffer.size() , 0);
                if(n >= 0) {
                    // n 表示实际发送的字节
                    _outbuffer.erase(0 , n);
                    if(_outbuffer.empty())
                        break;
                } else {
                    // 写事件这里的 errno == EAGAIN 表示的意思是发送缓冲区已满
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    else if(errno == EINTR)
                        continue;
                    else {
                        except();
                        return;
                    }
                }
            }

            // 1.数据发送完毕
            // 2.发送缓冲区已满，发送条件不具备
            if(!_outbuffer.empty()) {
                // 开启对写事件的关心
                getReactorPtr()->openWriteReadEvents(_sockfd , true , true);
            } else {
                // 关闭对写事件的关心
                getReactorPtr()->openWriteReadEvents(_sockfd , false , true);
            }
        }

        virtual void except() override {
            // 处理读出错、写出错、客户端关闭
            // 本质都是关闭链接
            getReactorPtr()->delConnection(_sockfd);
        }
    private:    
        int _sockfd;
        std::string _inbuffer;
        std::string _outbuffer;
        InetAddr _client_addr;
};