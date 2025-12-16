#pragma once

#include <unistd.h>
#include <sys/epoll.h>
#include "Log.hpp"

enum EpollerExit{
    EPOLL_CREATE_ERROR = 1,
    EPOLL_CTL_ERROR,
    EPOLL_WAIT_ERROR
};

class Epoller{
    const static int default_fd = -1;
    public:
        Epoller()
            :_epfd(default_fd)
        {
            int n = epoll_create(1024);
            if(n < 0) {
                LogModule::LOG(LogModule::LogLevel::FATAL) << "epoll create error";
                exit(EPOLL_CREATE_ERROR);
            }
            LogModule::LOG(LogModule::LogLevel::INFO) << "epoll create success - epfd: " << _epfd;
        }

        void addEvent(int fd , uint16_t events) {
            struct epoll_event* ep;
            ep->data.fd = fd;
            ep->events = events; 
            int n = epoll_ctl(_epfd , EPOLL_CTL_ADD , fd , ep);
            if(n < 0) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "epoll ctl error";
            }
        }

        int waitEvent(struct epoll_event* events , int maxevents , int timeout) {
            int n = epoll_wait(_epfd , events , maxevents , timeout);
            if(n < 0) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "epoll wait error";
                return -1;
            } else if(n == 0) {
                LogModule::LOG(LogModule::LogLevel::WARNING) << "epoll waitout";
                return -2;
            }
            return n;
        }

        ~Epoller() {
            if(_epfd > 0)
                close(_epfd);
        }
    private:
        int _epfd;
};