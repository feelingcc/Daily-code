#pragma once

#include <vector>
#include "Log.hpp"
#include "InetAddr.hpp"

using namespace LogModule;

#define QUIT_MESSAGE "quit"

// 这是一个给当前在线用户消息转发的类
class Transfer{
    bool is_first_online(const InetAddr& addr) {
        // 检测当前用户是否是刚上线用户
        for(size_t i = 0; i < _online_users.size(); i++) {
            if(_online_users[i] == addr) {
                // 已经存在
                return false;
            }
        }
        return true;
    }
    public:
        Transfer() = default;

        void forwardToOnlieUsers(int transfd , const std::string& message , const InetAddr& addr) {
            if(is_first_online(addr)) {
                _online_users.emplace_back(addr);   // 添加新用户
                LOG(LogLevel::INFO) << addr.showIpPort() << " 用户上线啦！";
            }

            std::string transmit_message = addr.showIpPort() + "# " + message;

            // 向所有在线用户发送消息
            for(auto& user : _online_users) {
                sendto(transfd , transmit_message.c_str() , transmit_message.size() , 0 , (sockaddr*)&user.getInetAddr() , sizeof(user.getInetAddr()));
            }

            // 可能有用户需要下线
            if(message == QUIT_MESSAGE) {
                std::vector<InetAddr>::iterator iter = _online_users.begin();
                while(iter != _online_users.end()) {
                    if(*iter == addr) {
                        _online_users.erase(iter);
                        break;  // 删除后立即break，防止迭代器失效
                    }
                }
            }
        }
    private:
        std::vector<InetAddr> _online_users;    // 保存每一个上线用户，第一次发送消息代表上线
};