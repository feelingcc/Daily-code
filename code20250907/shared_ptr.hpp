#pragma once
#include <iostream>

namespace simulate_shared_ptr {
    template<typename T>
    class shared_ptr {
        public:
            // 构造的时候初始化引用计数
            shared_ptr<T>(T* _ptr) :ptr(_ptr) ,  pcount(new int(1)) {}
            shared_ptr<T>(const shared_ptr<T>& sp) :ptr(sp.ptr) , pcount(sp.pcount) { ++(*pcount); }
            shared_ptr<T>& operator=(const shared_ptr<T>& sp) {
                // 1.相同对象之间的赋值 p1 = p1;
                // 2.不同对象，但管理的资源相同之间的赋值 p1 = p2;
                // 3.不同对象，管理的资源也不同之间的赋值 p3 = p4;
                
                // 判断两个对象管理的不是同一个资源再相互赋值
                if (ptr != sp.ptr) {
                    if (--(*pcount) == 0) {
                        delete ptr;
                        delete pcount;
                    }
                    ptr = sp.ptr;
                    pcount = sp.pcount;
                    ++(*pcount);
                }
                return *this;
            }

            ~shared_ptr<T>() {
                // 引用计数减到0释放空间
                if (--(*pcount) == 0) {
                    delete ptr;
                    delete pcount;
                }
            }
            T& operator*() { return *ptr; }
            T* operator->() { return ptr; }
            T& operator[] (size_t pos) { return ptr[pos]; }
        private:
            T* ptr;
            int* pcount;
    };
}