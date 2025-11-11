#pragma once
#include <iostream>
#include <string>
#include "Socket.hpp"

// 基于网络版本的计算器
class Request{
    public:
        Request(int x , int y , char op)
            :left(x)
            ,right(y)
            ,oper(op)
        {}

        std::string serialization() {

        }

        bool deserialization(const std::string& data) {

        }

    private:
        int left;
        int right;
        char oper;
};

class Response{
    public:
        Response(int res = 0 , bool _valid = false) 
            :result(res)
            ,valid(_valid)
        {}

        std::string serialization() {

        }

        bool deserialization(const std::string& data) {

        }
    private:
        int result;
        bool valid;
};

// 协议类，需要解决两个问题
//   1. 需要有序列化和反序列化功能
//   2. 对于Tcp必须保证读到完整的报文
class Protocol{
    public:
        void getClientAccept(std::shared_ptr<Socket>& client_accpet , const InetAddr& client_addr) {

        }
    private:

};