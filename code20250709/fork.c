#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
    printf("父进程开始运行，pid = %d\n" , getpid());
    pid_t id = fork();
    if (id < 0) {
        perror("fork");
        return -1;
    } else if (id == 0) {
        // child process
        while (1) {
            sleep(1);
            printf("我是一个子进程，我的id = %d，我的父进程id = %d\n" , getpid() , getppid());
        }
    } else {
        // parent process
        while (1) {
            sleep(1);
            printf("我是一个子进程，我的id = %d，我的父进程id = %d\n" , getpid() , getppid());
        }
    }

    return 0;
}