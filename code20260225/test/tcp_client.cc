#include "../server.hpp"

int main()
{
    signal(SIGPIPE , SIG_IGN);
    Socket cli_sock;
    cli_sock.createClient(8500, "127.0.0.1");
    for (int i = 0; i < 5; i++) {
        std::string str = "ccc";
        // std::cout << "发送的数据：" << str << std::endl;
        cli_sock.send(str.c_str(), str.size());
        char buf[1024] = {0};
        ssize_t n = cli_sock.recv(buf, 1023);
        // std::cout << n << std::endl;
        DEBUG_LOG("[收到服务器的数据]: %s", buf);
        sleep(1);
    }
    while(1) sleep(1);
    return 0;
}