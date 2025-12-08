#include "SelectServer.hpp"

// selectserver port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        std::cout << argv[0] << " port" << std::endl;
        exit(USAGE_ERROR);
    }

    uint16_t port = std::stoi(argv[1]);

    LogModule::ENABLE_CONSOLE_FLUSH_STRATEGY();

    std::unique_ptr<SelectServer> select_server = std::make_unique<SelectServer>(port);
    select_server->start();

    return 0;
}