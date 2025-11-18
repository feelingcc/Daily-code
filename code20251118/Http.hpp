#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "TcpServer.hpp"

const std::string default_space = " ";  // 空格
const std::string default_sep = "\n";   // 换行

class HttpRequest{
    public:
        std::string serialization() {
            return std::string();
        }

        bool deserialization(const std::string& serial_str) {
            return true;
        }
    private:
        std::string _req_method;    // 请求方法
        std::string _url;           // url  ·
        std::string _version;       // http版本
        std::unordered_map<std::string , std::string> _req_header;  // 请求报头 [key: value]
        std::string _blank_line;    // 空行
        std::string _text;          // 请求正文
};

class Http{
    public:
        Http(u_int16_t port) 
            :_tsvrp(std::make_unique<TcpServer>(port))
        {}

        void hanlderHttpRequest(std::shared_ptr<Socket>& client , const InetAddr& client_addr) {
            #ifndef DEBUG
            #define DEBUG
                std::string res;
                client->recv(&res); // 浏览器发送的是一个Http字符串
                std::cout << res;
            #endif  
        }

        void start() {
            _tsvrp->start([this](std::shared_ptr<Socket>& client , const InetAddr& client_addr){
                this->hanlderHttpRequest(client , client_addr);
            });
        }
    private:
        std::unique_ptr<TcpServer> _tsvrp;
};