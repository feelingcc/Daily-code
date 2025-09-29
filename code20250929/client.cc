#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "comm.hpp"
#include <cstring>

int main () {

    int fd = open(PIPE_NAME , O_WRONLY | O_TRUNC);
    if (fd < 0) {
        std::cerr << "open file error" << std::endl;
        return -1;
    }
    char buffer[1024] = {0};
    while (true) {
        std::cout << "Please Enter: ";
        std::cin >> buffer;
        write(fd , buffer , strlen(buffer));
    }
    close(fd);


    return 0;
}