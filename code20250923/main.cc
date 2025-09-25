#include "processPool.hpp"

int main () {

    // test();

    ProcessPool pp;
    pp.create();
    pp.run(9);
    pp.destroy();

    return 0;
}