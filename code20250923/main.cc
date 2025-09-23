#include "processPool.hpp"

// void test () {
//     Tasks t;
//     while (true) {
//         std::cout << t.get_taskcode() << std::endl;
//     }

//     // srand(time(nullptr));
//     // while (true) {
//     //     std::cout << rand() << std::endl;
//     // }
// }

int main () {

    // test();

    ProcessPool pp;
    pp.create();
    pp.run(9);
    pp.destroy();

    return 0;
}