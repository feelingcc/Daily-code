#include "UdpServer.hpp"
#include "Transfer.hpp"
#include "ThreadPool.hpp"
#include <memory>

#define USAGE_EXIT 1

using thread_handler_t = std::function<void()>;

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

    // 2.服务器的线程池类
    ThreadPool<thread_handler_t>* tp = ThreadPool<thread_handler_t>::getInstance();

    // 2.udp网络通信类，将收到的消息交给线程池处理
    std::unique_ptr<UdpServer> udp_server = std::make_unique<UdpServer>(port , [&transfer , &tp](int transfd ,const std::string& message ,const InetAddr& addr) {
        thread_handler_t thread_handler = std::bind(&Transfer::forwardToOnlieUsers , &transfer , transfd , message , addr);
        tp->enqueue(thread_handler);  // udp server 收到消息之后将数据和处理数据的函数打包为一个新函数（线程可以处理的函数类型）只需要入队列即可
    });
    udp_server->init();
    udp_server->start();

    // 线程回收
    tp->stop();
    tp->join(); 

    return 0;
}