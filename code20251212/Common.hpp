#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include <functional>

enum ExitCode{
    OK = 0,
    SOCKET_ERROR,
    BIND_ERROR,
    LISTEN_ERROR,
    USAGE_ERROR,
    CONNECT_ERROR,
    FORK_ERRO
};

class NoCopy{
    public:
        NoCopy() = default;
        ~NoCopy() = default;
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator=(const NoCopy&) = delete;
};

// 设置文件描述符为非阻塞
void setNonBlock(int fd) {
    int fl = fcntl(fd , F_GETFL);
    if(fl < 0) {
        perror("fcntl");
        return;
    }
    fcntl(fd, F_SETFL , fl | O_NONBLOCK);
}