
#include "shm.hpp"
#include "namedpipe.hpp"

int main() {

    // 创建共享内存
    IpcShm shm;
    // 创建命名管道
    // 使用管道解决共享内存的数据不一致问题
    NamedPipe namedpipe;
    PipeCommunicator pc;
    pc.openForRead();
    char* shm_mem = (char*)shm.getVirtualAddr();

    while(true) {
        if (pc.reader()) { // 在 reader 中阻塞
            std::cout << shm_mem << std::endl;
        } else {
            // client exit
            break;
        }
    }

    return 0;
}