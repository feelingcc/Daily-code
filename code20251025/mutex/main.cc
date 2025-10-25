#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include "mutex.hpp"

struct ThreadDate{
    ThreadDate(const std::string& name , Mutex& mutex) 
        :_thread_name(name)
        ,_mutex(mutex)
    {}
    std::string _thread_name;
    Mutex& _mutex;
};

int ticket = 100;
void *route(void *arg)
{
    ThreadDate* self = static_cast<ThreadDate*>(arg);
    while (1)
    {
        // self->_mutex.lock();
        MutexGuard mg(self->_mutex);
        if (ticket > 0)
        {
            usleep(1000);
            printf("%s sells ticket:%d\n", self->_thread_name.c_str(), ticket);
            ticket--;
            // self->_mutex.unlock();
        }
        else
        {   
            // self->_mutex.unlock();
            break;
        }
    }

    return nullptr;
}

int main(void)
{
    pthread_t t1, t2, t3, t4;

    Mutex mutex;

    ThreadDate* td1 = new ThreadDate("thread - 1"  , mutex);
    pthread_create(&t1, NULL, route, (void*)td1);

    ThreadDate* td2 = new ThreadDate("thread - 2"  , mutex);
    pthread_create(&t2, NULL, route, (void *)td2);

    ThreadDate* td3 = new ThreadDate("thread - 3"  , mutex);
    pthread_create(&t3, NULL, route, (void *)td3);

    ThreadDate* td4 = new ThreadDate("thread - 4"  , mutex);
    pthread_create(&t4, NULL, route, (void *)td4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}