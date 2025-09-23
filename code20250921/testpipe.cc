#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

void CWrite (int wfd) {
    char buffer[1024] = {0};
    int cnt = 0;
    while (true) {
        snprintf(buffer , sizeof(buffer) , "I am child process , pid: %d , cnt: %d" , getpid() , cnt++);
        write(wfd , buffer , strlen(buffer));
        sleep(1);
    }
}

void FRead (int rfd) {
    char buffer[1024] = {0};
    while (true) {
        int n = read(rfd , buffer , sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = 0;
        }
        std::cout << "I received a news " << buffer << std::endl;
        // sleep(1);
    }
}

int main () {
    
    // fds[0]: read fds[1]: write
    int fds[2] = {0};
    int n = pipe(fds);
    if (n < 0) {
        std::cerr << "open pipe error" << std::endl;
        return -1;
    }
    n = fork();
    if (n == 0) {
        // child process
        // 单向通信，子进程关闭读端
        close(fds[0]);
        CWrite(fds[1]);
        close(fds[1]);
        exit(0);
    }
    // 单向通信，父进程关闭写端
    close(fds[1]);
    FRead(fds[0]);
    close(fds[0]);
    waitpid(n , nullptr , 0);

    return 0;
}
