#include "../server.hpp"

// void CloseHandler(Channel* channel) {
//     DEBUG_LOG("close: %d" ,channel->Fd() );
//     // std::cout << "close: " << channel->Fd() << std::endl;
//     channel->Remove();
//     delete channel;
// }

// void ReadHandler(Channel* channel) {
//     char buffer[1024] = {0};
//     int n = recv(channel->Fd() , buffer , 1023 , 0);
//     if(n <= 0) {
//         if(errno == EAGAIN || errno == EINTR) {
//             return;
//         }
//         ERROR_LOG("channel recv failed");
//         CloseHandler(channel);
//         return;
//     }
//     DEBUG_LOG("%s" , buffer);
//     channel->EnableWrite();
// }

// void WriteHandler(Channel* channel) {
//     const char* str = "今天天气真好！！";
//     int n = send(channel->Fd() , str , strlen(str) , 0);
//     if(n <= 0) {
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
//     loop->RefreshTimeTask(id);
// }


std::unordered_map<uint64_t , std::shared_ptr<Connection>> conns;
int conn_id = 0;

void HandlerMessage(const std::shared_ptr<Connection>& conn , Buffer* out) {
    DEBUG_LOG("%s" , out->ReadPosition());
    out->MoveReadOffset(out->ReadableSize());
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

void Acceptor(EventLoop* loop , Channel* listen_channel) {
    int accept_fd = accept(listen_channel->Fd() , nullptr , nullptr);
    if(accept_fd < 0) {
        ERROR_LOG("listen accept failed");
        return;
    }
    conn_id++;
    std::shared_ptr<Connection> conn(new Connection(conn_id , loop , accept_fd));
    conn->SetMessageCallBack(HandlerMessage);
    conn->SetServerClosedCallBack(HandlerServerClosed);
    conn->SetConnectedCallBack(HandlerConnected);
    conn->EnableInactiveRelease(10);  // 开始定时器
    conn->Established(); // 开启监听当前Conncetion的读事件
    conns.insert(std::make_pair(conn_id , conn));// 添加管理信息
}

int main()
{
    
    srand(time(nullptr));
    Socket srv_sock;
    srv_sock.CreateServer(8500);
    EventLoop loop;
    Channel listen_channel(&loop , srv_sock.Fd());
    listen_channel.SetReadCallback(std::bind(Acceptor , &loop , &listen_channel));
    listen_channel.EnableRead();
    while(true) {
        loop.Start();
    }
    
    return 0;
}