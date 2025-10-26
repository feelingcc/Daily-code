#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

const int default_maxsize = 5;

template<typename T>
class BlockQueue{
    public:
        BlockQueue(size_t maxsize = default_maxsize) :_maxsize(maxsize) {
            pthread_mutex_init(&_mutex , nullptr);
            pthread_cond_init(&_full_cond , nullptr);
            pthread_cond_init(&_empty_cond , nullptr);
        }

        // 生产者
        void production(const T& data) {
            pthread_mutex_lock(&_mutex);
            // 不能使用if注意点，会造成伪唤醒
            //     1. pthread_cond_wait调用失败立即返回，导致该线程继续向下执行 _q.push(data) 会向已经满的队列中继续添加，造成问题
            //     2. 假设现在队列中只有一个空位,但是消费者通过 pthread_cond_broadcast 唤醒所有等待的进程,多个生产者都依次竞争到锁，并且向下执行 q.push() 会向已经满的队列中继续添加，造成问题
            while(_q.size() == _maxsize) {
                // 生产满的时候生产者需要等待
                // 1.pthread_cond_wait会维护一个阻塞队列，当生产满的时候，该线程会到阻塞队列中排队等待，并且释放锁
                // 2.线程被唤醒时，会从pthread_cond_wait返回，但是返回的位置就已经在临界区内部了，所以pthread_cond_wait会在底层解决等待后并且申请锁成功才会返回
                pthread_cond_wait(&_full_cond , &_mutex);
            }
            _q.push(data);
            // 来到这个地方，一定是有数据的，通知消费者进行消费
            // 这样写即使 _empty_cond 阻塞队列中没有消费者也不影响
            pthread_cond_signal(&_empty_cond);
            pthread_mutex_unlock(&_mutex);
        }

        // 消费者
        T consumption() {
            pthread_mutex_lock(&_mutex);
            while(_q.empty()) {
                // 消费完的时候消费者需要等待
                // std::cout << "消费者进入阻塞队列中等待"
                pthread_cond_wait(&_empty_cond , &_mutex);
            }
            T data = _q.front();
            _q.pop();
            pthread_cond_signal(&_full_cond);
            pthread_mutex_unlock(&_mutex);
            return data;
        }

        ~BlockQueue() {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_full_cond);
            pthread_cond_destroy(&_empty_cond);
        }
    private:
        std::queue<T> _q;   
        unsigned int _maxsize;
        pthread_mutex_t _mutex;
        pthread_cond_t _full_cond;  // 生产者生产满的条件变量
        pthread_cond_t _empty_cond; // 消费者消费空的条件变量
};