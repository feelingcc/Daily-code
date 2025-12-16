#pragma once

#include <iostream>
#include <sys/epoll.h>

class Reactor;

class Connection{
    public:
        Connection()
            :_reactor(nullptr)
            ,_events(0)
        {}

        // get/set方法
        void setReactorPtr(Reactor* reactor) { _reactor = reactor; }
        Reactor* getReactorPtr() { return _reactor; }
        void setEvents(uint32_t events) { _events = events; }
        uint32_t getEvents() { return _events; }

        // 纯虚方法
        virtual int getSockfd() = 0;
        virtual void recver() = 0;
        virtual void sender() = 0;
        virtual void except() = 0;
    private:    
        // 1.回指指针
        Reactor* _reactor;
        // 2.监听的事件
        uint32_t _events;
};



