#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>

#define PIPE_PATHNAME "./mkfifo"
// #define EXIT_FAILURE 1

class NamedPipe {
    public:
        NamedPipe(const std::string pathname = PIPE_PATHNAME) :_pathname(pathname) {
            umask(0);
            // 创建命名管道
            int n = mkfifo(_pathname.c_str() , 0666);
            if (n < 0) {
                std::cerr << "create namedpipe error" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "create namedpipe success" << std::endl;
        }

        ~NamedPipe() {
            int n = unlink(_pathname.c_str());
            if (n == -1) {
                std::cerr << "unlink namedpipe error" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "unlink namedpipe success" << std::endl;
        }
    private:
        std::string _pathname;
};

class PipeCommunicator {
    public:
        PipeCommunicator(const std::string pathname = PIPE_PATHNAME) :_pathname(pathname) , _fd(-1) {}

        void openForRead() {
            // write 方没有执行open的时候，read方，就要在open内部进行阻塞
            // 直到有人把管道文件打开了，open才会返回
            _fd = open(_pathname.c_str() , O_RDONLY);
            if (_fd < 0) {
                std::cerr << "open namedpipe error" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "open namedpipe success" << std::endl;
        }

        void openForWrite() {
            _fd = open(_pathname.c_str() , O_WRONLY);
             if (_fd < 0) {
                std::cerr << "open namedpipe error" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "open namedpipe success" << std::endl;
        }

        bool reader() {
            char c;
            int n = read(_fd , &c, 1);
            if(n == 0) {
                // client exit
                return false;
            }
            printf("server端被唤醒: %d\n" , n);
            return true;
        }

        void writer() {
           char c = 'a';
           int n = write(_fd , &c , 1);
           printf("通知server端可以读取了: %d\n" , n);
        }

        ~PipeCommunicator() {
            if(_fd > 0) {
                close(_fd);
            }
        }
    private:
        std::string _pathname;
        int _fd;
};