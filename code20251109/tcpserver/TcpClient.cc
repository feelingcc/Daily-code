#include "Common.hpp"
#include "InetAddr.hpp"

void Usage(const std::string& out) {
    std::cerr << "Usage: " << out << " server_ip serever_port" << std::endl;;
    exit(USAGE_ERROR);
}

// tcpclient server_ip server_port
int main(int argc , char* argv[]) {

    if(argc != 3) {
        Usage(argv[0]);
    }

    std::string server_ip = argv[1];
    u_int16_t server_port = std::stoi(argv[2]);

    int sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if(sockfd < 0) {
        std::cerr << "socket create error" << std::endl;
        exit(SOCKET_ERROR);
    }
    // 客户端需要bind，但是无需显示bind，操作系统会自动bind并采用随机端口的方式
    // 不需要bind，也不需要 listen 和 accpet
    InetAddr server_addr(server_ip , server_port);
    int n = connect(sockfd , server_addr.getSockaddr() , server_addr.getSockaddrLen());
    if(n < 0) {
        std::cerr << "connect error" << std::endl;
        exit(CONNECT_ERROR);
    }

    while(true) {
        std::string message;
        std::cout << "client input: ";
        std::getline(std::cin , message);
        int n = write(sockfd , message.c_str() , message.size());
        (void)n;
        char buffer[1024];
        n = read(sockfd , buffer , sizeof(buffer) - 1);
        if(n > 0) {
            buffer[n] = 0;
            std::cout << buffer << std::endl;
        }
    }

    close(sockfd);

    return 0;
}