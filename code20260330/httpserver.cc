#include <iostream>
#include "httplib.h"

void HelloWorld(const httplib::Request & req, httplib::Response & resp) {
    std::cout << req.method << std::endl;
    std::cout << req.path << std::endl;
    std::cout << req.body << std::endl;
    //打印头部字段信息
    for (auto it : req.headers) {
        std::cout << it.first << " = " << it.second << std::endl;
    }
    //打印查询字符串
    for (auto it : req.params) {
        std::cout << it.first << " = " << it.second << std::endl;
    }
    std::string html_body = "<html><body><h1>Hello World</h1></body></html>";
    // httplib 会自动添加 Content-Length 字段
    resp.set_content(html_body, "text/html");
    resp.status = 200;
}

int main() {

    httplib::Server server;
    server.Get("/hello" , HelloWorld);
    server.Get(R"(/numbers/(\d+))" , [](const httplib::Request & req, httplib::Response & resp){
        std::cout << req.method << std::endl;
        std::cout << req.path << std::endl;
        std::cout << req.body << std::endl;
        //打印头部字段信息
        for (auto it : req.headers) {
            std::cout << it.first << " = " << it.second << std::endl;
        }
        //打印查询字符串
        for (auto it : req.params) {
            std::cout << it.first << " = " << it.second << std::endl;
        }
        // 打印捕捉参数
        std::cout << req.matches[1] << std::endl;
        std::string numb = req.matches[1];
        std::string html_body = "<html><body><h1> " + numb  + "</h1></body></html>";
        resp.set_content(html_body, "text/html");
        resp.status = 200;
    });
    server.listen("0.0.0.0" , 8080);

    return 0;
}