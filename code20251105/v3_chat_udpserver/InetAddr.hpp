#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

// 解析IP地址和端口号的类
class InetAddr{
    public:
        // 这个构造函数用来将 struct sockaddr_in 结构体转换为 
        //      - 1.本地序列的字符串风格的点分十进制的IP 
        //      - 2.本地序列的整数端口
        InetAddr(const struct sockaddr_in& addr) 
            :_addr(addr)
        {
            _ip = inet_ntoa(addr.sin_addr);
            _port = ntohs(addr.sin_port);
        }

        const std::string& getIP() const { return _ip; }
        u_int16_t getPort() const { return _port; }
        const struct sockaddr_in& getInetAddr() const { return _addr; } 
        
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