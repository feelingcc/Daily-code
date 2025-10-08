#include "comm.hpp"

int main() {

    IpcShm shm;
    shm.create();
    sleep(5);
    shm.attach();
    sleep(5);
    shm.destroy();

    return 0;
}