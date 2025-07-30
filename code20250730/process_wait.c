#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {

    pid_t id = fork();
    if (id == 0) {
        // child process
        int count = 5;
        while (count--) {
            printf("I am a child process , pid: %d , ppid: %d\n" , getpid() , getppid());
            sleep(1);
        }
        exit(100);
    } else if (id > 0) {
        // father process
        int status = 0;
        while (1) {
            pid_t res = waitpid(id , &status , WNOHANG);
            if (res > 0) {
                // if (WIFEXITED(status)) {    // wifexited
                if ((status&0x7f) == 0) { 
                    // printf("child process exit code: %d\n" , WEXITSTATUS(status)); // wexitstatus
                    printf("wait success , child process exit code: %d\n" , (status >> 8)&0xff); // wexitstatus
                } else {
                    printf("signal code: %d\n" , status&0x7f);
                }
                break;
            } else if (res == 0) {
                printf("sleep 1s continuous wait\n");
                sleep(1);
            } else {
                perror("waitpid ");
                exit(1);
            }
        }
    } else {
        perror("fork ");
    }

    return 0;
}