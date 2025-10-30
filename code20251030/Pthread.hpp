#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <functional>
#include <cstdlib>
#include <utility>
#include "Mutex.hpp"

// 封装pthread
class Thread{
    using func_t = std::function<void()>;

    // 因为成员函数具有隐含的this指针, 不满足 void*(*start_routine)(void*),需要声明为静态函数
    // 但是 _routine 函数需要使用类内成员, 所以需要通过 args 把 this 指针传进来
    static void* _routine(void* args) {
        Thread* self = static_cast<Thread*>(args);
        self->_is_running = true;
        self->_method();
        self->_is_running = false;
        return nullptr;
    }

    public:
        // 引用折叠，既可以传左值也可以传右值，而右值引用的属性是左值
        template<typename T>
        Thread(T&& method) 
            :_tid(0)
            // 这里不能使用move的原因是不是如果外面传入一个左值，这里直接move掉，外面的对象可能就制空了
            ,_method(std::forward<T>(method))   // 使用完美转发保持右值属性，这样右值就会调用移动构造，左值继续调用拷贝构造
            ,_is_running(false)
            ,_is_detach(false)
        {
            int current;
            {
                MutexGuard mg(_id_lock);
                current = id++;
            }
            _tname = "thread - " + std::to_string(current);
        }

        bool start(bool dt = false) {
            if(_is_running)
                return false;   // 线程已经运行

            int n = pthread_create(&_tid , nullptr , _routine , (void*)this);
            if(n != 0) {
                std::cerr << "pthread_create error" <<std::endl;
                return false;
            }

            // 如果设置分离, 在启动后立即分离
            if(dt) {
                detach();
            }   
            std::cout << "pthread_create success , thread name: " << _tname << std::endl;
            return true;
        }

        void join() {
            // 若分离则不需要join
            if(_is_detach) {
                std::cout << "线程已经分离, 不能进行join" << std::endl;
                return;
            }

            int n = pthread_join(_tid , nullptr);
            if(n != 0) {
                std::cerr << "pthread_join error: " << n << std::endl;
            } else {
                std::cout << "pthread_join success , thread name: " << _tname << std::endl;
                _is_running = false;
            }
        }

        pthread_t gettid() { return _tid; }

        bool detach() { 
            // 若分离 / 未启动则不需要detach
            if(_is_detach || !_is_running) {
                std::cout << "线程已经分离或线程未启动, 不能进行detach" << std::endl;
                return false;
            }

            int n = pthread_detach(_tid);
            if(n != 0) {
                std::cerr << "pthread_detach error" << std::endl;
                return false;
            } else {
                std::cout << "pthread_detach success , thread name: " << _tname << std::endl;
                _is_detach = true;
                return true;
            }
        }

        // 谨慎使用 cancel，可能造成资源泄漏
        bool cancel() {
            if(_is_running) {
                int n = pthread_cancel(_tid);
                if(n != 0) {
                    std::cerr << "pthread_cancel error" << std::endl;
                    return false;
                } else {
                    std::cout << "pthread_cancel success" << std::endl;
                    _is_running = false;
                    return true;
                }
            }

            std::cout << "线程未启动" << std::endl;
            return false;
        }

        std::string& gettname() { return _tname; }

        ~Thread() {
            // if (_is_running && !_is_detach) {
            //     // 析构时如果线程还在运行且未分离，尝试join
            //     pthread_join(_tid, nullptr);
            // }
        }
    private:
        pthread_t _tid;         // 线程id
        std::string _tname;     // 线程名字
        func_t _method;         // 线程执行的方法
        bool _is_running;       // 线程是否在运行
        bool _is_detach;        // 线程是否分离
        // void* _result;    
        
        Mutex _id_lock;
    public:
        static int id;
};

int Thread::id = 1;