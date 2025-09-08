/*
    // 1.自己给自己赋值
    // p1 = p1;
    // 1.1 两个已存在的对象的赋值并且指向不同的空间
    // 1.1.1 
    // 1.2 两个已存在的对象的赋值并且指向一样的空间
    // 1.2.1 
    if (this != &sp) {
        // p1 = p2;
        if (pcount != sp.pcount) {
            ~shared_ptr();
            ptr = sp.ptr;
            pcount = sp.pcount;
            (*pcount)++;   
        }
    }
    return *this; 
*/

#include "shared_ptr.hpp"

#include <memory>
#include <string>
using std::string;
using std::cout;
using std::cin;
using std::endl;

void test() {
    std::shared_ptr<string> sp1(new string("111111"));
    std::shared_ptr<string> sp2(sp1);
    std::weak_ptr<string> wp = sp1;
    cout << wp.expired() << endl;   // 0
    cout << wp.use_count() << endl; // 2
    // sp1和sp2都指向了其他资源，则weak_ptr就过期了
    sp1 = std::make_shared<string>("222222");   
    cout << wp.expired() << endl;   // 0
    cout << wp.use_count() << endl; // 1
    sp2 = std::make_shared<string>("333333");
    cout << wp.expired() << endl;   // 1
    cout << wp.use_count() << endl; // 0
    wp = sp1;
    //std::shared_ptr<string> sp3 = wp.lock();
    auto sp3 = wp.lock();   
    cout << wp.expired() << endl;   // 0
    cout << wp.use_count() << endl; // 2
}

int main () {

    test();

    return 0;
}