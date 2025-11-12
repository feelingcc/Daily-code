#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "Calculate.hpp"

// tcpserver port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        LogModule::LOG(LogModule::LogLevel::FATAL) << argv[0] << " port";
        exit(USAGE_ERROR);
    }

    uint16_t server_port = std::stoi(argv[1]);

    // 1.应用层处理上层业务
    Calculate cal;
    // 2.表示层负责收发数据并进行序列化和反序列化
    Protocol protocol([&cal](const Request& req)->Response{
        cal.execute(req);
    });
    // 3.会话层负责建立与客户端的链接
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(server_port , 
        [&protocol](std::shared_ptr<Socket>& client_accpet , const InetAddr& client_addr){
            protocol.getClientAccept(client_accpet,client_addr);
        });

    return 0;
}