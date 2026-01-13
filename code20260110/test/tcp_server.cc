#include "../server.hpp"

void CloseHandler(Channel* channel) {
    std::cout << "close: " << channel->Fd() << std::endl;
    channel->Remove();
    delete channel;
}

void ReadHandler(Channel* channel) {
    char buffer[1024] = {0};
    int n = recv(channel->Fd() , buffer , 1023 , 0);
    if(n <= 0) {
        if(errno == EAGAIN || errno == EINTR) {
            return;
        }
        ERROR_LOG("channel recv failed");
        CloseHandler(channel);
        return;
    }
    std::cout << buffer << std::endl;
    channel->EnableWrite();
}

void WriteHandler(Channel* channel) {
    const char* str = "今天天气真好！！";
    int n = send(channel->Fd() , str , strlen(str) , 0);
    if(n <= 0) {
        if(errno == EAGAIN || errno == EINTR) {
            return;
        }
        ERROR_LOG("channel send failed");
        CloseHandler(channel);
        return;
    }
    channel->DisableWrite();
}

void ErrorHandler(Channel* channel) {
    CloseHandler(channel);
}

void EventHandler(Channel* channel) {
    INFO_LOG("这是一个任意事件");
}

void Acceptor(EventLoop* loop , Channel* listen_channel) {
    int accept_fd = accept(listen_channel->Fd() , nullptr , nullptr);
    if(accept_fd < 0) {
        ERROR_LOG("listen accept failed");
        return;
    }
    Channel* channel = new Channel(loop , accept_fd);
    channel->SetReadCallback(std::bind(ReadHandler , channel));
    channel->SetWriteCallback(std::bind(WriteHandler , channel));
    channel->SetCLoseCallback(std::bind(CloseHandler , channel));
    channel->SetErrorCallback(std::bind(ErrorHandler , channel));
    channel->SetEventCallback(std::bind(EventHandler , channel));
    channel->EnableRead();
}

int main()
{
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