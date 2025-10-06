#include "comm.hpp"

int main () {

    PipeCommunicator pc;
    pc.openForWrite();
    pc.writer();

    return 0;
}