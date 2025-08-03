#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

// #define DEBUG
#define HOSENAME_SIZE 128
#define PROMPT "*"
#define SEP "@"
#define FORMAT "%s" SEP "%s: %s" PROMPT " "
#define RECEIVE_SIZE 128
#define ARGV_SIZE 128

// 命令行参数表
char *argv[ARGV_SIZE];
int argc;

const char *getUsername()
{
    return getenv("USER");
}

const char *getHostname()
{
    static char hostname[HOSENAME_SIZE] = {0};
    gethostname(hostname, sizeof hostname);
    return hostname;
}

const char *getCurrentWorkDir()
{
    return getenv("PWD");
}

std::string getLastPathName(const std::string &path)
{
    if (path == "/")
        return "/";

    size_t pos = path.rfind("/");
    return path.substr(pos + 1);
}

void showCommandLine()
{
    printf(FORMAT, getUsername(), getHostname(), getLastPathName(getCurrentWorkDir()).c_str());
    // 刷新缓冲区
    fflush(stdout);
}

void splitCommand(char *str)
{
    // 重置
    memset(argv, 0, sizeof argv);
    argc = 0;

    argv[argc] = strtok(str, " ");
    while (argv[argc] != nullptr)
    {
        argc++;
        argv[argc] = strtok(nullptr, " ");
    }
}

bool receiveCommand(char receive_buffer[], int size)
{
    // receive_buffer 会通过 strtok 处理，但是 receive_buffer 是局部变量出了函数就销毁了
    // 所以 strtok 切割之后 argv 指针数组中每个元素指向已销毁的 receive_buffer 空间
    // char receive_buffer[RECEIVE_SIZE] = {0};

    char *res = fgets(receive_buffer, size, stdin);
    if (res == nullptr)
        return false;

    // 用户会回车确认，删除回车
    res[strlen(res) - 1] = 0;

    // 只输入一个回车，则什么都不做
    if (!strlen(res))
        return false;

#ifdef DEBUG
    printf("%s\n", res);
#endif

    // 切割命令
    splitCommand(res);

#ifdef DEBUG
    for (int i = 0; argv[i]; i++)
        printf("%s\n", argv[i]);
    printf("argc = %d\n", argc);
#endif

    return true;
}

int latest_exitcode;
void executeNormalCommand()
{
    pid_t id = fork();
    if (id == 0)
    {
        execvp(argv[0], argv);
        perror("execvp failed");
        exit(1);
    }
    int status = 0;
    pid_t res = waitpid(id, &status, 0);
    if (res > 0)
    {
        // 记录最近一个程序的退出码
        latest_exitcode = WEXITSTATUS(status);
    }
}

void cdBuildCommand()
{
    if (argc == 1)
    {
        // 默认进入家目录
        const char *home = getenv("HOME");
        chdir(home);
    }
    else if (argc == 2)
    {   
        if (!strcmp(argv[1] , "~"))
        {
            const char *home = getenv("HOME");
            chdir(home);
        }
        else 
        {
            chdir(argv[1]);
        }
    }
    else
    {
        printf("argc illegal\n");
        exit(1);
    }

    // 保存环境变量的内容使用全局变量，否则有意想不到的错误
    static char cwdenv[128] = {0};
    static char cwd[128] = {0};
    memset(cwdenv , 0 , sizeof cwdenv);
    memset(cwd , 0 , sizeof cwd);
    char* res = getcwd(cwd , sizeof cwd);
    if (res == nullptr) {
        perror("getcwd");
        exit(1);
    }
    // 更新环境变量
    snprintf(cwdenv , sizeof cwdenv , "PWD=%s" , res);
    putenv(cwdenv);
}

bool executeBuildCommand(const char *command)
{
    if (!strcmp(command, "cd"))
    {
        cdBuildCommand();
        return true;
    }
    // ...
    return false;
}

int main()
{
    // printf("%s" , getenv("PATH"));

    while (true)
    {
        // 1.打印命令行提示符
        showCommandLine();
        // 2.接收用户输入
        char receive_buffer[RECEIVE_SIZE] = {0};
        if (!receiveCommand(receive_buffer, sizeof receive_buffer))
            continue;
        // 3. 执行内建命令
        if (executeBuildCommand(argv[0]))
            continue;
        // 4.执行普通命令
        executeNormalCommand();
    }

    return 0;
}
