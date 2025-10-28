#pragma once

#include <semaphore.h>

class Sem{
    const unsigned int defaultvalue = 1;
    public:
        Sem(unsigned int sem_value) {
            sem_init(&_sem , 0 , sem_value);
        }

        // --
        void P() {
            int n = sem_wait(&_sem);    // 原子的
            (void)n;
        }

        // ++
        void V() {
            int n =sem_post(&_sem);     // 原子的
            (void)n;
        }

        ~Sem() {
            sem_destroy(&_sem);
        }
    private:
        sem_t _sem;
};