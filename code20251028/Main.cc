#include "RingQueue.hpp"

#include <string>
#include <memory>
#include <ctime>
#include <unistd.h>

const int producer_num = 2;
const int consumer_num = 3;

struct ThreadDate{
    ThreadDate(const std::string& name , RingQueue<int>* ringq) 
        :thread_name(name)
        ,rq(ringq)
    {}
    std::string thread_name;
    RingQueue<int>* rq;
};

void* producer(void* args) {
    ThreadDate* tdata = static_cast<ThreadDate*>(args);
    while(true) {
        int num = rand() % 10;
        std::cout << tdata->thread_name << " 生产了一个数据: " << num << std::endl;
        tdata->rq->production(num);
    }

    delete tdata;
    return nullptr;
}

void* consumer(void* args) {
    ThreadDate* tdata = static_cast<ThreadDate*>(args);
    while(true) {
        sleep(1);
        int res = 0;
        tdata->rq->consumption(&res);
        std::cout << tdata->thread_name << " 消费了一个数据: " << res << std::endl;
    }

    delete tdata;
    return nullptr;
}

int main() {

    srand(time(nullptr));
    
    RingQueue<int>* ringqueue = new RingQueue<int>;

    pthread_t ptids[producer_num] , ctids[consumer_num];
    for(int i = 0; i < producer_num; i++) {

        ThreadDate* td = new ThreadDate("producer-thread-" + std::to_string(i + 1) , ringqueue);

        pthread_create(ptids + i , nullptr , producer , td);
    }

    for(int i = 0; i < consumer_num; i++) {
        ThreadDate* td = new ThreadDate("consumer-thread-" + std::to_string(i + 1) , ringqueue);

        pthread_create(ctids + i , nullptr , consumer , td);
    }

    for(int i = 0; i < producer_num; i++) {
        pthread_join(ptids[i] , nullptr);
    }

    for(int i = 0; i < consumer_num; i++) {
        pthread_join(ctids[i] , nullptr);    
    }

    delete ringqueue;

    return 0;
}