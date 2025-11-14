#pragma once
#include <iostream>
#include <string>
#include "Socket.hpp"
#include <jsoncpp/json/json.h>  // 引入jsoncpp第三方库

using namespace SocketMoudle;

// 基于网络版本的计算器
class Request{
    public:
        Request() = default;

        Request(int x , int y , char op)
            :left(x)
            ,right(y)
            ,oper(op)
        {}

        std::string serialization() {
            Json::Value root;
            root["x"] = left;
            root["y"] = right;
            root["oper"] = oper;
            Json::StyledWriter writer;
            return writer.write(root);
        }

        bool deserialization(const std::string& data) {
            Json::Value root;
            Json::Reader reader;
            bool ok = reader.parse(data , root);
            if(!ok) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "request deserialization error";
                return false;
            }
            left = root["x"].asInt();
            right = root["y"].asInt();
            oper = root["oper"].asInt();
            return true;
        }

        int get_x() const { return left; }
        int get_y() const { return right; }
        char get_oper() const { return oper; }

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
            Json::Value root;
            root["result"] = result;
            root["valid"] = valid;
            Json::StyledWriter writer;
            return writer.write(root);
        }

        bool deserialization(const std::string& data) {
            Json::Value root;
            Json::Reader reader;
            bool ok = reader.parse(data , root);
            if(!ok) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "response deserialization error";
                return false;
            }
            result = root["result"].asInt();
            valid = root["valid"].asBool();
            return true;
        }

        void showResult() {
            std::cout << "result: " << result << " [valid:" << valid << "]" << std::endl;
        }
    private:
        int result;
        bool valid;
};

using calculate_t = std::function<Response(const Request&)>;

// 协议类，需要解决两个问题
//   1. 需要有序列化和反序列化功能
//   2. 对于Tcp必须保证读到完整的报文
const static std::string sep = "\r\n";
// format: 10\r\n{"x":10, "y":20, "oper":"+"}\r\n
class Protocol{
    public:
        Protocol() = default;   
        Protocol(calculate_t cal_handler) :_cal_handler(cal_handler) {}

        // 添加报头
        std::string encode(const std::string& jsonstr) {
            std::string json_len = std::to_string(jsonstr.size());
            return json_len + sep + jsonstr + sep;
        }

        // 分离报头
        bool decode(std::string& buffer_queue , std::string& res) {
            size_t pos = buffer_queue.find(sep);
            if(pos == std::string::npos) {
                return false;
            }
            std::string json_len = buffer_queue.substr(0 , pos); // 有效载荷总长度
            int packet_len = json_len.size() + std::stoi(json_len) +  2 * sep.size();
            if(packet_len > buffer_queue.size()) {
                return false;   //说明当前读取的数据不足一个完整的报文，读取失败，应该继续读取
            }
            // 来到这里，当前已经有一个完整的报文或者多个完整的报文，或者一个半报文
            res = buffer_queue.substr(json_len.size() + sep.size() , std::stoi(json_len));   //将有效载荷带回上层
            // 将整个报文从buffer_queue分离
            buffer_queue.erase(0 , packet_len);
            return true;
        }

        void getClientAccept(std::shared_ptr<Socket>& client_accpet , const InetAddr& client_addr) {
            LogModule::LOG(LogModule::LogLevel::INFO) << "accept success client: " << client_addr.showIpPort();
            std::string buffer;
            while(true) {
                int n = client_accpet->recv(&buffer);
                if(n > 0) {
                    // 1.可能读到不是一个完整的报文 decode为false内层循环不进去，执行外层循环继续读取
                    // 2.也可能读取到多个完整的报文 decode为true内层循环持续处理多个完整报文
                    std::string jsonstr;
                    while(decode(buffer , jsonstr)) {
                        // 3.服务端接收客户端的计算任务，需要反序列化
                        Request req;
                        req.deserialization(jsonstr);
                        // 4.将反序列化的结果交给上层计算模块处理
                        Response res = _cal_handler(req);
                        // 5.将计算后的结果序列化
                        std::string resp_json = res.serialization();
                        // 6.将序列化后的json字符串添加报头
                        std::string packet = encode(resp_json);
                        // 7.发送给客户端
                        client_accpet->send(packet);
                    }
                } else if(n == 0) {
                    // client quit
                    LogModule::LOG(LogModule::LogLevel::INFO) << "client: " << client_addr.showIpPort() << " quit";
                    break;
                } else {
                    LogModule::LOG(LogModule::LogLevel::ERROR) << "server recv error";
                    break;
                }
            }
        }

        std::string buildClientRequest(int x, int y , char oper) {
            // 1.构建客户端请求
            Request req(x , y , oper);
            // 2.序列化
            std::string json_req = req.serialization();
            // 3.添加报头   
            return encode(json_req);
        }

        bool getServerResponse(std::shared_ptr<Socket>& client ,std::string& buffer_queue, Response* resq) {
            while(true) {
                // 可能读取到不是一个完整的报文 
                int n = client->recv(&buffer_queue);
                if(n > 0) {
                    std::string json_response;
                    bool ok = decode(buffer_queue , json_response);
                    if(!ok)
                        continue;   //不是一个完整的报文，继续读取
                    while(ok) {
                        // 保证了肯定有一个完整的报文，但是可能会有多个，所以需要连续处理
                        // 4.反序列化
                        resq->deserialization(json_response);
                        // 5.显示结果
                        resq->showResult();
                        // sleep(100); debug
                        ok = decode(buffer_queue , json_response);
                    }
                    return true;
                } else if(n == 0) {
                    // server quit
                    LogModule::LOG(LogModule::LogLevel::INFO) << "server quit";
                    return false;
                } else {
                    LogModule::LOG(LogModule::LogLevel::INFO) << "recv error";
                    return false;
                }
            }
        }
    private:
        calculate_t _cal_handler;
};