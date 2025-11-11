#include "TcpServer.hpp"
#include "Protocol.hpp"

// tcpserver port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        LogModule::LOG(LogModule::LogLevel::FATAL) << argv[0] << " port";
        exit(USAGE_ERROR);
    }

    uint16_t server_port = std::stoi(argv[1]);

    Protocol protocol;
    
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(server_port , 
        [&protocol](std::shared_ptr<Socket>& client_accpet , const InetAddr& client_addr){
            protocol.getClientAccept(client_accpet,client_addr);
        });

    return 0;
}