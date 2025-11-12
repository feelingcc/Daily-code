#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

const std::string any_ip = "0.0.0.0";

// 解析IP地址和端口号的类
class InetAddr{
    public:
        InetAddr() = default;
        // 这个构造函数用来将 struct sockaddr_in 结构体转换为 
        //      - 1.本地序列的字符串风格的点分十进制的IP 
        //      - 2.本地序列的整数端口
        // 网络转主机
        InetAddr(const struct sockaddr_in& addr) 
            :_addr(addr)
        {
            _port = ntohs(addr.sin_port);
            char ip_buffer[64];
            inet_ntop(AF_INET , &addr.sin_addr , ip_buffer, sizeof(ip_buffer));
            _ip = ip_buffer;
        }

        void setSockaddrIn(const struct sockaddr_in& addr) {
            _addr = addr;
            _port = ntohs(addr.sin_port);
            char ip_buffer[64];
            inet_ntop(AF_INET , &addr.sin_addr , ip_buffer, sizeof(ip_buffer));
            _ip = ip_buffer;
        }

        // 主机转网络
        // #define INADDR_ANY 0
        InetAddr(const std::string ip , u_int16_t port) 
            :_ip(ip)
            ,_port(port)
        {
            memset(&_addr , 0 , sizeof(_addr));
            _addr.sin_family = AF_INET;
            _addr.sin_port = htons(_port);
            inet_pton(AF_INET , _ip.c_str() , &_addr.sin_addr);
        }

        InetAddr(u_int16_t port) 
            :_port(port)
            ,_ip(any_ip)
        {
            memset(&_addr , 0 , sizeof(_addr));
            _addr.sin_family = AF_INET;
            _addr.sin_port = htons(_port);
            _addr.sin_addr.s_addr = INADDR_ANY; 
            // inet_pton(AF_INET , _ip.c_str() , &_addr.sin_addr);
        }

        const std::string& getIP() const { return _ip; }
        u_int16_t getPort() const { return _port; }
        const struct sockaddr_in& getSockaddrin() const { return _addr; } 
        const struct sockaddr* getSockaddr() const { return (const struct sockaddr*)&_addr; }
        struct sockaddr* getSockaddr() { return (struct sockaddr*)&_addr; }
        socklen_t getSockaddrLen() const { return sizeof(_addr); }
        
        // 格式化显示IP + Port
        std::string showIpPort() const {
            return "[" + _ip + " : " + std::to_string(_port) + "]";
        }

        bool operator==(const InetAddr& addr) const {
            return _ip == addr.getIP() && _port == addr.getPort(); 
        }
    private:
        struct sockaddr_in _addr;
        std::string _ip;
        u_int16_t _port;
};