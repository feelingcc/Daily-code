#include "../server.hpp"

int main()
{
    Socket srv_sock;
    srv_sock.createServer(8500);
    int acceptfd = srv_sock.accept();

    while(1) {
        char buf[1024] = {0};
        ssize_t n = ::recv(acceptfd , buf, 1023 , 0);
        if(n <= 0) {
            std::cout << "close fd: " << acceptfd << std::endl;
            ::close(acceptfd);
            break;
        }
        std::cout << "client: " << buf << std::endl;
        std::string resp = "今天天气真不错";
        ::send(acceptfd , resp.c_str() , resp.size() , 0);
    }
    while(1) sleep(1);
    srv_sock.close();
    return 0;
}