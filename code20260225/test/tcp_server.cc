#include "../server.hpp"

void CloseHandler(Channel* channel) {
    DEBUG_LOG("close: %d" ,channel->fd() );
    // std::cout << "close: " << channel->Fd() << std::endl;
    channel->Remove();
    delete channel;
}

void ReadHandler(Channel* channel) {
    char buffer[1024] = {0};
    int n = recv(channel->fd() , buffer , 1023 , 0);
    if(n <= 0) {
        if(errno == EAGAIN || errno == EINTR) {
            return;
        }
        ERROR_LOG("channel recv failed");
        CloseHandler(channel);
        return;
    }
    DEBUG_LOG("[收到客户端数据]: %s" , buffer);
    channel->EnableWrite();
}

void WriteHandler(Channel* channel) {
    const char* str = "今天天气真好！！";
    int n = send(channel->fd() , str , strlen(str) , 0);
    if(n < 0) {
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

void EventHandler(EventLoop* loop ,uint64_t id , Channel* channel) {
    // DEBUG_LOG("EventHandler 执行了");
    loop->refreshTimeTask(id);
}

void acceptor(EventLoop* loop , Channel* lst_channel) {
    int newfd = accept(lst_channel->fd() , nullptr , nullptr);
    if(newfd < 0) return;

    uint64_t timerid = rand() % 10000;
    Channel* channel = new Channel(loop, newfd);
    channel->setReadCallback(std::bind(ReadHandler , channel));
    channel->setWriteCallback(std::bind(WriteHandler , channel));
    channel->setCLoseCallback(std::bind(CloseHandler , channel));
    channel->setErrorCallback(std::bind(ErrorHandler , channel));
    channel->setEventCallback(std::bind(EventHandler ,loop ,timerid , channel));

    loop->addTimeTask(timerid , 10 , std::bind(CloseHandler , channel));
    channel->EnableRead();
}

int main() {
    srand(time(nullptr));
    Socket lst_sock;
    lst_sock.createServer(8500);
    EventLoop loop;
    Channel channel(&loop , lst_sock.fd());
    channel.setReadCallback(std::bind(acceptor , &loop , &channel));
    channel.EnableRead();

    INFO_LOG("开启事件监控，等待事件就绪");
    while(1) {
        loop.start();
    }
    
    lst_sock.close();
    return 0;
}