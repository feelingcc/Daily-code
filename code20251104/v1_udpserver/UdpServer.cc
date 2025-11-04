#include "UdpServer.hpp"
#include <memory>

// 上层处理数据
std::string handler(const std::string& str) {
    std::string res = "server echo: ";
    res += str;
    return res;
}

#define USAGE_EXIT 1

// ip地址使用 0.0.0.0 无需绑定具体ip
// ./udpserver port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        exit(USAGE_EXIT);
    }

    u_int16_t port = std::stoi(argv[1]);

    ENABLE_CONSOLE_FLUSH_STRATEGY();

    std::unique_ptr<UdpServer> udp_server = std::make_unique<UdpServer>(port , handler);
    udp_server->init();
    udp_server->start();

    return 0;
}