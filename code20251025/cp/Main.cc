#include "BlockQueue.hpp"
#include <ctime>


void* producer(void* args) {
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    while(true) {
        int num = rand();
        bq->production(num);
        std::cout << "生产者生产了一个数字: " << num << std::endl;
        sleep(1);   // 生产的慢一点，消费的快一点
    }

    return nullptr;
}

void* customer(void* args) {
    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);

    while(true) {
        int res = bq->consumption();
        std::cout << "消费者消费了一个数字: " << res << std::endl;
    }

    return nullptr;
}

int main() {

    srand(time(nullptr));

    BlockQueue<int>* bq = new BlockQueue<int>;

    pthread_t t1 , t2;
    pthread_create(&t1 , nullptr , producer , bq);
    pthread_create(&t2 , nullptr , customer , bq);

    pthread_join(t1 , nullptr);
    pthread_join(t2 , nullptr);

    return 0;
}