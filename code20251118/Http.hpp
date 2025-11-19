#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "TcpServer.hpp"
#include "Utils.hpp"

const std::string default_space = " ";  // 空格
const std::string default_sep = "\n";   // 换行
const std::string default_kvsep = ": "; // key和value的分隔符

const std::string webroot = "./wwwroot";   
const std::string homepage = "index.html";

class HttpRequest{
    public:
        std::string serialization() {
            return std::string();
        }

        bool deserialization(std::string& serial_request) {
            // 1.处理请求行
            std::string request_line;
            bool res = Utils::ReadOnlyLineStr(serial_request , &request_line);
            (void)res;
            std::stringstream ss(request_line);
            ss >> _req_method >> _req_uri >> _req_version;
            LogModule::LOG(LogModule::LogLevel::DEBUG) << "method: " << _req_method << " "
                                                       << "uri: " << _req_uri << " "
                                                       << "version: " << _req_version;
            if(_req_uri == "/")
                _req_uri += webroot + _req_uri + homepage;
            else 
                _req_uri = webroot + _req_uri;
            // ...

            return true;
        }

        std::string getUri(){ return _req_uri; }
    private:
        std::string _req_method;        // 请求方法
        std::string _req_uri;           // url  ·
        std::string _req_version;       // http版本
        std::unordered_map<std::string , std::string> _req_header;  // 请求报头 [key: value]
        std::string _blank_line;        // 空行
        std::string _req_text;          // 请求正文
};

class HttpResponse{
    public:
        std::string serialization() {
            std::string response_serial;
            std::string status_line = _resq_version + default_space + std::to_string(_resq_code) + default_space + _code_desc + default_sep;
            response_serial += status_line;
            for(auto& kv : _resq_header) {
                std::string header = kv.first + default_kvsep + kv.second + default_sep;
                response_serial += header;
            }
            response_serial += _blank_line;
            response_serial += _resq_text;
            return response_serial;
        }

        bool deserialization(std::string& serial_str) {
            return true;
        }

        void setTargetSource(const std::string& target_source) {
            _target_source = target_source;
        }

        bool makeResponse() {
            // 根据请求的uri判断资源是否存在
            bool res = Utils::ReadFileContent(_target_source , &_resq_text);
            if(!res) {
                _resq_code = 404;
                _code_desc = "Not Found";
                return false;
            }
            return true;
        }
    private:
        std::string _resq_version;      // http版本
        int _resq_code;                 // 状态码
        std::string _code_desc;         // 状态码描述
        std::unordered_map<std::string , std::string> _resq_header; // 响应报头
        std::string _blank_line = default_sep;        // 空行
        std::string _resq_text;         // 响应正文

        std::string _target_source;     // 客户端请求的目标资源
};

class Http{
    public:
        Http(u_int16_t port) 
            :_tsvrp(std::make_unique<TcpServer>(port))
        {}

        void hanlderHttpRequest(std::shared_ptr<Socket>& client , const InetAddr& client_addr) {
            std::string res;
            int n = client->recv(&res); // 这里可能会接收到不完整的报文或多组报文，这里需要做处理
            // 假设接收到了一个完整的http报文
            if(n > 0) {
                HttpRequest request;
                request.deserialization(res);
                HttpResponse response;
                response.setTargetSource(request.getUri());
                response.makeResponse();
                std::string response_serial = response.serialization();
                client->send(response_serial);
            }

            #ifdef DEBUG
            // #define DEBUG
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