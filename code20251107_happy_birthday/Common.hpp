#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <unistd.h>

enum ExitCode{
    OK = 0,
    SOCKET_ERROR,
    BIND_ERROR,
    LISTEN_ERROR,
    USAGE_ERROR
};

class NoCopy{
    public:
        NoCopy() = default;
        ~NoCopy() = default;
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator=(const NoCopy&) = delete;
};