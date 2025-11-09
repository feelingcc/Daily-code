#pragma once

#include <pthread.h>
#include "Mutex.hpp"

class Cond{
    public:
        Cond() {
            pthread_cond_init(&_cond , nullptr);
        }

        // 基于该锁下的条件变量进行等待
        void wait(Mutex& mutex) {
            int n = pthread_cond_wait(&_cond , mutex.getMutex());
            (void)n;
        }

        void signal() {
            int n = pthread_cond_signal(&_cond);
            (void)n;
        }

        void broadcast() {
            int n = pthread_cond_broadcast(&_cond);
            (void)n;
        }

        ~Cond() {
            pthread_cond_destroy(&_cond);
        }
    private:
        pthread_cond_t _cond;
};