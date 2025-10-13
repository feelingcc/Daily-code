#include <iostream>
#include <signal.h>
#include <unistd.h>

void print_pending(const sigset_t& pending) {
    std::cout << "process pid: " << getpid() << ", pending: ";
    for(size_t i = 31; i >= 1; i--) {
        std::cout << sigismember(&pending , i);
    }
    std::cout << std::endl;
}

void handler_sigint(int signo) {
    std::cout << "我正在处理" << signo << "号信号" << std::endl;
}

// demo
int main() {

    signal(SIGINT , handler_sigint);

    sigset_t set , oset;
    sigemptyset(&set);
    sigemptyset(&oset);

    // 设置2号信号阻塞
    sigaddset(&set , SIGINT);  
    sigprocmask(SIG_SETMASK , &set , &oset);

    // 查看 pending 表
    sigset_t pending;
    int cnt = 0;
    while (true) {
        sigemptyset(&pending);
        sigpending(&pending);
        print_pending(pending);
        if (cnt == 15) {
            // 解除屏蔽
            sigprocmask(SIG_SETMASK , &oset , nullptr);
            // 观察信号被递达的过程
        }
        cnt++;
        sleep(1);
    }

    return 0;
}

// void handler_sigchld(int signo) {
//     std::cout << "我收到了: " << signo << "信号" << std::endl;
// }

// sigset_t;

// int main () {

//     signal(SIGCHLD , handler_sigchld);
//     raise(SIGCHLD);
//     // 验证默认处理动作是忽略的信号，是否会执行 handler_sigchild函数
//     // 会执行，因为我使用 handler_sigchld 覆盖了默认处理函数

//     return 0;
// }