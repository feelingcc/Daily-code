#include "../server.hpp"

void HandlerMessage(const std::shared_ptr<Connection>& conn , Buffer* out) {
    DEBUG_LOG("%s" , out->readPosition());
    out->moveReadOffset(out->getReadableSize());
    std::string str = "hello world";
    conn->Send(str.c_str() , str.size());
    conn->Shutdown();   // 测试短链接
}

void HandlerClosed(const std::shared_ptr<Connection>& conn) {
    DEBUG_LOG("release connection %p" , conn.get());
}

void HandlerConnected(const std::shared_ptr<Connection>& conn) {
    DEBUG_LOG("new connection %p" , conn.get()); // 智能指针中的get是获取对象的地址
}

int main() {

    EventLoop baseloop;
    TcpServer server(&baseloop , 8500);
    server.SetConnectedCallBack(HandlerConnected);
    server.SetMessageCallBack(HandlerMessage);
    server.SetClosedCallBack(HandlerClosed);
    server.SetThreadCount(2);
    // server.EnableInactiveRelease(10);
    server.Start();

    return 0;
}