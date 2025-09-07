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

int main () {

    return 0;
}