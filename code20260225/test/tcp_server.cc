#include "../server.hpp"

// void CloseHandler(Channel* channel) {
//     DEBUG_LOG("close: %d" ,channel->fd() );
//     // std::cout << "close: " << channel->Fd() << std::endl;
//     channel->Remove();
//     delete channel;
// }

// void ReadHandler(Channel* channel) {
//     char buffer[1024] = {0};
//     int n = recv(channel->fd() , buffer , 1023 , 0);
//     if(n <= 0) {
//         if(errno == EAGAIN || errno == EINTR) {
//             return;
//         }
//         ERROR_LOG("channel recv failed");
//         CloseHandler(channel);
//         return;
//     }
//     DEBUG_LOG("[收到客户端数据]: %s" , buffer);
//     channel->EnableWrite();
// }

// void WriteHandler(Channel* channel) {
//     const char* str = "今天天气真好！！";
//     int n = send(channel->fd() , str , strlen(str) , 0);
//     if(n < 0) {
//         if(errno == EAGAIN || errno == EINTR) {
//             return;
//         }
//         ERROR_LOG("channel send failed");
//         CloseHandler(channel);
//         return;
//     }
//     channel->DisableWrite();
// }

// void ErrorHandler(Channel* channel) {
//     CloseHandler(channel);
// }

// void EventHandler(EventLoop* loop ,uint64_t id , Channel* channel) {
//     // DEBUG_LOG("EventHandler 执行了");
//     loop->refreshTimeTask(id);
// }

// void acceptor(EventLoop* loop , Channel* lst_channel) {
//     int newfd = accept(lst_channel->fd() , nullptr , nullptr);
//     if(newfd < 0) return;

//     uint64_t conn_id = rand() % 10000;
//     Connection* conn = new Connection(conn_id , loop, newfd);
//     conn->Established();

//     loop->addTimeTask(timerid , 10 , std::bind(CloseHandler , channel));
//     channel->EnableRead();
// }

std::unordered_map<uint64_t , std::shared_ptr<Connection>> conns;
int conn_id = 0;
EventLoop loop;

void HandlerMessage(const std::shared_ptr<Connection>& conn , Buffer* out) {
    DEBUG_LOG("%s" , out->readPosition());
    out->moveReadOffset(out->getReadableSize());
    std::string str = "hello world";
    conn->Send(str.c_str() , str.size());
    conn->Shutdown();   // 测试短链接
}

void HandlerServerClosed(const std::shared_ptr<Connection>& conn) {
    conns.erase(conn->Id());
}

void HandlerConnected(const std::shared_ptr<Connection>& conn) {
    DEBUG_LOG("new connection %p" , conn.get()); // 智能指针中的get是获取对象的地址
}

void HandlerNewConnection(int accept_fd) {
    conn_id++;
    std::shared_ptr<Connection> conn(new Connection(conn_id , &loop , accept_fd));
    conn->SetMessageCallBack(HandlerMessage);
    conn->SetServerClosedCallBack(HandlerServerClosed);
    conn->SetConnectedCallBack(HandlerConnected);
    conn->EnableInactiveRelease(10);  // 开始定时器
    conn->Established(); // 开启监听当前Conncetion的读事件
    conns.insert(std::make_pair(conn_id , conn));// 添加管理信息
}


int main() {
    
    Acceptor acceptor(&loop , 8500);
    acceptor.setAcceptCallback(HandlerNewConnection);
    acceptor.startListen();
    INFO_LOG("开启事件监控，等待事件就绪");
    while(1) {
        loop.start();
    }

    return 0;
}