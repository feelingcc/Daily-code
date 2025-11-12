#pragma once

#include "Protocol.hpp"

class Calculate{
    public:
        Response execute(const Request& req) {
            switch(req.get_oper()) {
                case '+':
                    return Response(req.get_x() + req.get_y() , true);
                case '-':
                    return Response(req.get_x() - req.get_y() , true);
                case '*':
                    return Response(req.get_x() * req.get_y() , true);
                case '/':
                    {
                        if(req.get_y() == 0)
                            return Response(0, false);
                        else 
                            return Response(req.get_x() / req.get_y() , true);
                    }
                default:
                    LogModule::LOG(LogModule::LogLevel::WARNING) << "未知的操作符";
            }
        }
};