#include "comm.hpp"

int main () {

    // 建立命名管道
    NamedPipe namedpipe;

    // 通信
    PipeCommunicator pc;
    pc.openForRead();
    pc.reader();

    return 0;
}