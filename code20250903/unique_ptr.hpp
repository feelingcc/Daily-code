#pragma once

namespace simulate_unique_ptr {
    // unique_ptr 不允许拷贝
    template <typename T>
    class unique_ptr {
        public:
            explicit unique_ptr(T* _ptr) :ptr(_ptr) {}
            unique_ptr(const unique_ptr<T>& self) = delete;
            unique_ptr<T>& operator=(const unique_ptr<T>& self) = delete;
            unique_ptr(unique_ptr<T>&& self) {
                ptr = self.ptr;
                self.ptr = nullptr;
            }
            ~unique_ptr() { 
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

