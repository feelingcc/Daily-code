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

        void reader() {
            char buffer[1024] = {0};
            while (true) {
                int n = read(_fd , buffer, sizeof(buffer) - 1);
                if (n > 0) {
                    buffer[n] = 0;
                    std::cout << "client send: " << buffer << std::endl;
                } else if (n == 0) {
                    std::cout << "client exit" << std::endl;
                    break; 
                } else {
                    std::cout << "server read error" << std::endl;
                    break;                
                }
            }
        }

        void writer() {
            std::string message;
            int count = 1;
            while (true) {
                std::cout << "Please Enter: ";
                std::getline(std::cin , message);
                message += (" , message number: " + std::to_string(count++));
                write(_fd , message.c_str() , message.size());
            }
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