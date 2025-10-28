#pragma once
#include <iostream>
#include <vector>
#include "Mutex.hpp"
#include "Sem.hpp"

template <typename T>
class RingQueue{
    const static unsigned int default_max_capacity = 5;
    public:
        RingQueue(unsigned int max_capacity = default_max_capacity) 
            :_ring_q(max_capacity)
            ,_max_capacity(max_capacity)
            ,_blank_sem(max_capacity)
            ,_p_step(0)
            ,_data_sem(0)
            ,_c_step(0)
        {}

        void production(const T& data) {
            // 多个线程先申请信号量
            _blank_sem.P();
            // 保证生产者之间的互斥关系
            {
                MutexGuard mg(_p_mutex);
                _ring_q[_p_step] = data;
                _p_step++;
                _p_step %= _max_capacity;
            }
            _data_sem.V();
        }

        void consumption(T* res) {
            _data_sem.P();
            // 临界区
            {
                MutexGuard mg(_c_mutex);
                *res = _ring_q[_c_step];
                _c_step++;
                _c_step %= _max_capacity;
            }
            _blank_sem.V();
        }
    private:
        std::vector<T> _ring_q;
        unsigned int _max_capacity;

        Sem _blank_sem;         // 生产者信号量，代表可生产的资源数量
        unsigned int _p_step;   // 生产者在该位置进行生产

        Sem _data_sem;          // 消费者信号量，代表可消费的资源数量
        unsigned int _c_step;   // 消费者在该位置进行消费

        Mutex _p_mutex;         // 保证生产者之间的互斥关系
        Mutex _c_mutex;         // 保证消费者之间的互斥关系
};