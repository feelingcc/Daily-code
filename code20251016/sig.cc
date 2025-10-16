#include <iostream>
#include <signal.h>

int flag = 0;

void handler_sig(int) {
    std::cout << "handler 2 signal" << std::endl;
    flag = 1;
}

int main() {

    signal(2 , handler_sig);
    // 死循环
    while(!flag) {

    }
    std::cout << "main process end!" << std::endl;

    return 0;
}

// #include <iostream>
// #include <signal.h>
// #include <unistd.h>

// // s.sa_mask 会将2号信号阻塞，也就是说打印pending时，发现2号信号为1，就说明被阻塞没有被执行。
// void handler_sigint(int signo) {
//     std::cout << "handler sigint 2 signal" << std::endl;
//     // 不断的打印 pending 表
//     sigset_t set;
//     sigemptyset(&set);  
//     while(true) {
//         sigpending(&set);
//         for(int i = 31; i >= 1; i--) {
//             std::cout << sigismember(&set , i);
//         }
//         std::cout << std::endl;
//         sleep(1);
//     }
// }

// int main() {

//     struct sigaction s;
//     s.sa_flags = 0;
//     s.sa_handler = handler_sigint;

//     // 在执行2号信号处理函数时，也可以设置其他信号的屏蔽
//     sigset_t mask;
//     sigemptyset(&mask);  
//     sigaddset(&mask , SIGQUIT);
//     s.sa_mask = mask;

//     sigaction(SIGINT , &s , nullptr);

//     while(true) {
//         std::cout << "hello world: " << getpid() << std::endl;
//         sleep(1);
//     }

//     return 0;
// }