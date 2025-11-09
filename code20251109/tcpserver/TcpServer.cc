#include "TcpServer.hpp"
#include "Command.hpp"

void Usage(const std::string& out) {
    LogModule::LOG(LogModule::LogLevel::INFO) << "Usage: " << out << " port";
    exit(USAGE_ERROR);
}

// tcpserver port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        Usage(argv[0]);
    }

    u_int16_t port = std::stoi(argv[1]);

    Command command;

    std::unique_ptr<TcpServer> tser = std::make_unique<TcpServer>(port , std::bind(&Command::execute , &command , std::placeholders::_1));
    tser->init();
    tser->start();

    return 0;
}