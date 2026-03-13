#include "../server.hpp"

class EchoServer {
    private:
        TcpServer _server;

        void HandlerMessage(const std::shared_ptr<Connection>& conn , Buffer* out) {
            conn->Send(out->readPosition() , out->getReadableSize());
            out->moveReadOffset(out->getReadableSize());
            conn->Shutdown();   // 测试短链接
        }

        void HandlerClosed(const std::shared_ptr<Connection>& conn) {
            DEBUG_LOG("release connection %p" , conn.get());
        }

        void HandlerConnected(const std::shared_ptr<Connection>& conn) {
            DEBUG_LOG("new connection %p" , conn.get()); // 智能指针中的get是获取对象的地址
        }
    public:
        EchoServer(uint16_t port) :_server(port) {
            _server.SetConnectedCallBack(std::bind(&EchoServer::HandlerConnected , this , std::placeholders::_1));
            _server.SetMessageCallBack(std::bind(&EchoServer::HandlerMessage , this , std::placeholders::_1 , std::placeholders::_2));
            _server.SetClosedCallBack(std::bind(&EchoServer::HandlerClosed , this , std::placeholders::_1));
            _server.SetThreadCount(2);
        }
        void start() {
            _server.Start();
        }
};
