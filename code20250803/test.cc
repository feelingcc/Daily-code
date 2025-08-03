#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

// #define DEBUG
#define HOSENAME_SIZE 128
#define PROMPT "*"
#define SEP    "@"
#define FORMAT "%s" SEP "%s: %s" PROMPT " "
#define RECEIVE_SIZE  128
#define ARGV_SIZE     128

// 命令行参数表
char* argv[128];
int argc;

void splitCommand(char* str) {
    // 重置
    memset(argv , 0 , sizeof argv);
    argc = 0;

    argv[argc] = strtok(str , " ");
    while (argv[argc] != NULL) {
        argc++;
        argv[argc] = strtok(NULL , " ");
    }
}

bool receiveCommand() {
    char receive_buffer[RECEIVE_SIZE] = {0};
    char* res = fgets(receive_buffer , sizeof receive_buffer , stdin);
    if (res == nullptr) 
        return false; 

    // 用户会回车确认，删除回车
    res[strlen(res) - 1] = 0;

    // 只输入一个回车，则什么都不做
    // if (!strlen(res))
    //     return false;

#ifdef DEBUG
    printf("%s\n" , res);
#endif

    // 切割命令
    splitCommand(res);

#ifdef DEBUG
    for (int i = 0; argv[i]; i++) printf("%s\n" , argv[i]);
    printf("argc = %d\n" , argc);
#endif

    return true;
}

int main () {

    printf("start\n");
    // char receive_buffer[128] = {0};
    // char* res = fgets(receive_buffer , sizeof receive_buffer , stdin);
    // // 用户会回车确认，删除回车
    // res[strlen(res) - 1] = 0;
    // splitCommand(receive_buffer);
    receiveCommand();
    pid_t id = fork();
    if (id == 0) {
        execvp(argv[0] , argv);
        perror("execvp failed");
        exit(1);
    }
    int status = 0;
    pid_t r = waitpid(id , &status , 0);

    printf("end\n");

    return 0;
}