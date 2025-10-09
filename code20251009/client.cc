
#include "shm.hpp"
#include "namedpipe.hpp"

int main() {

    IpcShm shm(USER);
    PipeCommunicator pc;
    pc.openForWrite();
    char* shm_mem = (char*)shm.getVirtualAddr();

    // sleep(5);

    int index = 0;
    for(char c = 'A'; c < 'E'; c++) {
        // AA 
        // AABB
        // AABBCC
        shm_mem[index++] = c;
        shm_mem[index++] = c;
        // 使用命名管道告诉server可以读取了
        pc.writer();
        sleep(1);
    }

    return 0;
}