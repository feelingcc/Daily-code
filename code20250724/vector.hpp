#pragma once

#include <iostream>
#include <cassert>

namespace simulateSTL{
template<typename T>
class vector{   // 类内部<T>可省略
    public:
        typedef T* iterator;
        typedef const T* const_iterator;
        iterator begin() { return start; } 
        iterator end() { return finish; }
        const_iterator begin() const { return start; }
        const_iterator end() const { return finish; }
        size_t size() const { return finish - start; }
        size_t capacity() const { return end_of_storage - start; }
        bool empty() const { return finish == start; }
        void clear() { finish = start; }
        T& operator[](size_t pos) { assert(pos < size()); return start[pos]; }
        const T& operator[](size_t pos) const { assert(pos < size()); return start[pos]; }
        void swap(vector<T>& v) { std::swap(start , v.start); std::swap(finish , v.finish); std::swap(end_of_storage , v.end_of_storage); }  
        vector<T>() = default;      
        vector<T>(const vector<T>& v) { for (auto& val : v) push_back(val); }
        vector<T>& operator=(vector<T> v) { swap(v); return *this; }
        ~vector<T>() { delete[] start; start = finish = end_of_storage = nullptr; } 
        template<typename InputIterator>  
        vector<T>(InputIterator first , InputIterator last) {
            while (first != last) {
                push_back(*first); 
                ++first;
            }
        }
        vector<T>(size_t n , const T& val = T()) {
            for (size_t i = 0; i < n; i++) push_back(val);
        }
        // 防止 vector<int> v(10); 调用 template<typename InputIterator>
        vector<T>(int n , const T& val = T()) {
            while (n--) push_back(val);
        }

        void reserve(size_t n);
        void resize(size_t n , const T& val = T());
        void push_back(const T& val);
        void pop_back();
        iterator insert(iterator position , const T& val);
        iterator erase (iterator position);
    private:
        iterator start = nullptr;
        iterator finish = nullptr;
        iterator end_of_storage = nullptr;
};


template<typename T>
void vector<T>::resize(size_t n , const T& val) {
    if (n < size()) {
        finish = start + n;
    } else {
        reserve(n);
        while (finish < start + n) { 
            *finish = val;
            finish++;
        }
    }
}


template<typename T> 
void vector<T>::reserve(size_t n) { 
    if (n > capacity()) { 
        size_t old_size = size();  
        T* temp = new T[n]; 
        for (size_t i = 0; i < size(); i++) {
            temp[i] = start[i];     
        }
        delete[] start;  
        start = temp;
        finish = temp + old_size;         
        end_of_storage = temp + n;
    }
}

template<typename T>
void vector<T>::push_back(const T& val) {
    if (finish == end_of_storage) {
        reserve(end_of_storage == nullptr ? 4 : capacity() * 2);
    }

    *finish = val;
    finish++;
}

template<typename T>
void vector<T>::pop_back() {
    assert(!empty());  
    finish--;
}

// insert 和 erase 注意迭代器失效问题！
template<typename T>
typename vector<T>::iterator vector<T>::insert(iterator position , const T& val) {
    assert(position >= start && position <= finish);  
    if (finish == end_of_storage) {
        size_t relative_pos = position - start;
        reserve(end_of_storage == nullptr ? 4 : capacity() * 2);
        position = start + relative_pos;
    }
    iterator end = finish;
    while (end > position) {
        *end = *(end - 1);
        end--;
    }
    *position = val;
    finish++;

    return position; // 返回新插入位置的迭代器
}
  
template<typename T>  
typename vector<T>::iterator vector<T>::erase (iterator position) {     
    assert(position >= start && position < finish);
    iterator end = position + 1;
    while (end < finish) {
        *(end - 1) = *end;
        end++;
    }
    finish--;
    return position; // 返回删除位置的下一个迭代器
}

} // namespace simulateSTL