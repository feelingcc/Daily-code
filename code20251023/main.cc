#include "thread.hpp"

int main() {

    Thread t([](){
        while(true) {
            std::cout << "I am a new thread" << std::endl;
            sleep(1);
        }
    });

    t.start();
    sleep(5);

    t.detach();
    sleep(5);

    t.cancel();
    sleep(5);

    t.join();

    while(true) {
        sleep(1);
        std::cout << "I am main thread" << std::endl;
    }

    return 0;
}