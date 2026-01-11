#include "../server.hpp"

int main()
{
    Socket srv_sock;
    srv_sock.CreateServer(8500);
    int accept_fd = srv_sock.Accept();
    std::cout << accept_fd << std::endl;
    Socket accept_sock(accept_fd);
    while(true) {
        char buf[1024] = {0};
        int n = accept_sock.Recv(buf , 1023);
        std::cout << n << " " << strerror(errno) << std::endl;
        INFO_LOG("client: %s" , buf);
        std::string echo = "server: ";
        echo += buf;
        accept_sock.Send(echo.c_str() , echo.size());
    }
    
    return 0;
}