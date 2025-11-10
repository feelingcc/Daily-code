#pragma once
#include <iostream>
#include <string>

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