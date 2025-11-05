#include "UdpServer.hpp"
#include "Transfer.hpp"
#include <memory>

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

    // 1.创建消息转发的类
    Transfer transfer;

    // 2.udp网络通信类，两个类通过回调函数交互
    std::unique_ptr<UdpServer> udp_server = std::make_unique<UdpServer>(port , [&transfer](int transfd ,const std::string& message ,const InetAddr& addr) {
        transfer.forwardToOnlieUsers(transfd , message , addr);
    });
    udp_server->init();
    udp_server->start();

    return 0;
}