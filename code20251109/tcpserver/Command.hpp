#pragma once
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

std::vector<std::string> default_whitelist = {"ls" , "pwd" , "who" , "whoami" , "ll" , "ls -l"};

class Command{
    public:
        Command() :_white_list(default_whitelist.begin() , default_whitelist.end()) {}

        std::string execute(const std::string& cmd) {
            // 判断是否在白名单中的命令
            std::set<std::string>::iterator iter = _white_list.find(cmd);
            if(iter == _white_list.end()) {
                // 不存在
                return "invalid command!\n";            
            }

            int pipefd[2];  // pipe[0]: read , pipe[1]: write
            // 1.创建管道
            int n = pipe(pipefd);
            if(n < 0) {
                std::cerr << "pipe create error" << std::endl;
                exit(1);
            }
            // 2.创建子进程
            pid_t pid = fork();
            if(pid < 0) {
                std::cerr << "fork error" << std::endl;
                exit(2);
            } else if(pid == 0) {
                // child process write
                close(pipefd[0]);   
                // 3.将子进程的标准输出重定向到管道的写端
                dup2(pipefd[1] , 1);
                close(pipefd[1]);
                // 4.解析命令
                std::vector<std::string> tokens;
                std::stringstream ss(cmd == "ls" ? "ls -C" : cmd);
                std::string token;
                while(ss >> token) {
                    tokens.emplace_back(token);
                }
                std::vector<char*> args;
                for(auto& t: tokens) {
                    args.emplace_back(const_cast<char*>(t.c_str()));
                }
                args.push_back(nullptr);
                
                // 5.程序替换执行命令
                execvp(args[0] , args.data());
                exit(0);
            }
            // parent process read
            close(pipefd[1]);
            char buffer[1024];
            ssize_t count = read(pipefd[0] , buffer , sizeof(buffer) - 1);
            if(count > 0) {
                buffer[count] = 0;
            } else if(count < 0) {
                waitpid(pid , nullptr , 0);
                std::cerr << "read error" << std::endl;
                exit(3);
            }
            // 6.回收子进程
            waitpid(pid , nullptr , 0);
            close(pipefd[0]);
            return buffer;
        }
    private:
        std::set<std::string> _white_list;
};