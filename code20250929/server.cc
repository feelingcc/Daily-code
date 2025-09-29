#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "comm.hpp"

int main () {

    umask(0);
    // 创建命名管道
    int n = mkfifo(PIPE_NAME , 0666);
    if (n < 0) {
        std::cerr << "create mkfifo error" << std::endl;
        return -1;
    }
    int fd = open(PIPE_NAME , O_RDONLY);
    char buffer[1024] = {0};
    while (true) {
        int n = read(fd , buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = 0;
        } else if (n == 0) {
            std::cout << "client exit" << std::endl;
            break; 
        }
        std::cout << "client send: " << buffer << std::endl;
    }
    close(fd);


    return 0;
}