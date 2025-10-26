#pragma once

#include <pthread.h>

class Mutex{
    public:
        Mutex() {
            pthread_mutex_init(&_mutex , nullptr);
        }

        void lock() {
            int n = pthread_mutex_lock(&_mutex);
            (void)n;
        }

        void unlock() {
            int n = pthread_mutex_unlock(&_mutex);
            (void)n;
        }

        pthread_mutex_t* getMutex() { return &_mutex; }

        ~Mutex() {
            pthread_mutex_destroy(&_mutex);
        }
    private:
        pthread_mutex_t _mutex;
};
// 基于 RAII 的互斥锁
class MutexGuard{
    public:
        MutexGuard(Mutex& mutex)
            :_mutex(mutex)
        {
            _mutex.lock();
        }
        ~MutexGuard() {
            _mutex.unlock();
        }
    private:
        Mutex& _mutex;
};