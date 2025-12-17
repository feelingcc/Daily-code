
#include "Calculate.hpp"
#include "Protocol.hpp"
#include "Listener.hpp"
#include "Reactor.hpp"

// reactorServer port
int main(int argc , char* argv[]) {

    if(argc != 2) {
        std::cout << argv[0] << " port" << std::endl;
        exit(USAGE_ERROR);
    }

    uint16_t port = std::stoi(argv[1]);

    LogModule::ENABLE_CONSOLE_FLUSH_STRATEGY();

    // 1. 应用层
    Calculate cal;

    // 2. 表示层
    Protocol protocol([&cal](const Request& req) -> Response {
        LogModule::LOG(LogModule::LogLevel::DEBUG) << "应用层处理报文";
        return cal.execute(req);
    });

    // 3. 会话层
    std::shared_ptr<Connection> listener_ptr = std::make_shared<Listener>(port);
    listener_ptr->registerCallback([&protocol](std::string& inbuffer)->std::string{
        LogModule::LOG(LogModule::LogLevel::DEBUG) << "表示层处理报文开始";
        // 1.能否读取出一个完整的报文
        // 2.能否处理多个报文
        std::string outbuffer;
        while(true) { 
            std::string request_json;
            if(!protocol.decode(inbuffer , request_json))  // 不存在完整的报文
                break;
            // 来到这里，response_json 里100%存在一个完整的报文
            outbuffer += protocol.execute(request_json);
        }
        LogModule::LOG(LogModule::LogLevel::DEBUG) << "表示层处理报文结束";
        return outbuffer;
    });
    
    std::unique_ptr<Reactor> reactor_server_ptr = std::make_unique<Reactor>();
    reactor_server_ptr->addConnection(listener_ptr);
    reactor_server_ptr->start();


    return 0;
}