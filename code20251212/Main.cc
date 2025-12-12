#include "Reactor.hpp"
#include "Connection.hpp"

// reactorServer port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        std::cout << argv[0] << " port" << std::endl;
        exit(USAGE_ERROR);
    }

    uint16_t port = std::stoi(argv[1]);

    LogModule::ENABLE_CONSOLE_FLUSH_STRATEGY();

    std::unique_ptr<Reactor> reactor_server_ptr = std::make_unique<Reactor>();
    std::shared_ptr<Connection> listener_ptr = std::make_shared<Listener>(port);


    return 0;
}