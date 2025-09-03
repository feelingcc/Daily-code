#pragma once

namespace simulate_auto_ptr {
    template <typename T>
    class auto_ptr {
        public:
            auto_ptr(T* _ptr) :ptr(_ptr) {}
            auto_ptr(auto_ptr<T>& self) {
                if (ptr) delete[] ptr;
                ptr = self.ptr;
                self.ptr = nullptr;
            }
            auto_ptr<T>& operator=(auto_ptr<T>& self) {
                if (this != &self) {
                    if (ptr) delete[] ptr;
                    ptr = self.ptr;
                    self.ptr = nullptr;
                }
                return *this;
            }
            ~auto_ptr() { 
                if (ptr)
                    delete[] ptr;
            }

            T& operator* () { return *ptr; }
            T* operator-> () { return ptr; }
            T& operator[] (size_t pos) { return ptr[pos]; }
        private:
            T* ptr;
    };
}
