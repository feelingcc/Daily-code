#include "vector.hpp"

void test1 () {
    simulateSTL::vector<int> v;
}

int main () {

    // test1();
    int a = int(0);
    // 这样调用 int 的析构函数是不对的。int 是内置类型，没有析构函数，不能这样调用。
    // a.~int(); // 错误用法

    return 0;
}