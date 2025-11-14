#include "Socket.hpp"
#include "Protocol.hpp"

int main(int argc , char* argv[]) {

    if(argc != 3) {
        std::cout << argv[0] << " server_ip server port" << std::endl;
        exit(USAGE_ERROR);
    }

    std::string server_ip = argv[1];
    u_int16_t server_port = std::stoi(argv[2]);
    InetAddr server_addr(server_ip , server_port);

    std::shared_ptr<SocketMoudle::Socket> sptr = std::make_shared<SocketMoudle::TcpSocket>();
    sptr->buildTcpClientListendSocket();
    sptr->connect(server_addr); //  与服务器建立连接

    Protocol protocol;

    std::string buffer_queue;
    while(true) {
        int x , y;
        char oper;
        std::cout << "Please input x: ";
        std::cin >> x;
        std::cout << "Please input y: ";
        std::cin >> y;
        std::cout << "Please input operator: ";
        std::cin >> oper;
        // debug
        // std::cout << "x: " << x << " y: " << y << " oper: " << oper << std::endl;
        // 1.构建客户端请求
        std::string packet = protocol.buildClientRequest(x , y , oper);
        // 2.向服务器发送   
        sptr->send(packet);
        // 3.接收服务器返回结果
        Response resq;
        bool ok = protocol.getServerResponse(sptr ,buffer_queue, &resq);
        if(!ok) {
            break;
        }
    }

    sptr->close();

    return 0;
}