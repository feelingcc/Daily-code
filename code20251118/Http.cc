#include "Http.hpp"

int main(int argc , char* argv[]) {

    if(argc != 2) {
        std::cout << argv[0] << " port" << std::endl;
        exit(USAGE_ERROR);
    }
    u_int16_t server_port = std::stoi(argv[1]);
    
    std::unique_ptr<Http> http_server = std::make_unique<Http>(server_port);
    http_server->start();

    return 0;
}