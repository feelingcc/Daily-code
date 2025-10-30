#pragma once

#include <queue>
#include <vector>
#include "Pthread.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"
#include "Log.hpp"

using namespace LogModule;

const unsigned int default_num = 5;

template<typename T>
class ThreadPool{
    // 每个线程所执行的任务函数，从任务队列中取任务
    void handlerTask() {
        // 取任务的时候，线程池可能关闭了，但是线程需要把队列中的任务全部处理完，才能停止
        while(true) {
            T task;
            {
                MutexGuard mg(_task_mutex);
                while(_q.empty() && _is_running) { // while防止伪唤醒
                    // 1.如果队列为空并且线程池在运行则等待
                    // 2.使用到了短路，若队列不为空，则短路，则继续往下进行处理任务
                    // 3.若队列为空且线程池不运行，则退出while循环，进入下面的if条件
                    LOG(LogLevel::INFO) << "队列为空，线程进入阻塞";
                    _thread_wait_num++;
                    _task_cond.wait(_task_mutex);
                    _thread_wait_num--;
                }
                // 如果线程被唤醒，发现线程池关闭且队列中没有任务则退出循环
                if(_q.empty() && !_is_running) {
                    break;
                }
                task = _q.front();
                _q.pop();
            }
            task(); // 处理任务在临界区外
        }
    } 

    ThreadPool(unsigned int num = default_num)
            :_num(num)
            ,_is_running(false)
        {
            for(size_t i = 0; i < _num; i++) {
                // 这里传 Lambda 是因为 Thread中的任务类型是 std::function<void()>，而 handlerTask 默认第一个参数是 this，所以多包装一层
                // 而 Lambda 变量可见性规则是默认什么都看不到，包括成员变量和成员函数，所以需要传递this指针
                // emplace_back 无需拷贝或移动，直接原地构造
                _threads.emplace_back(
                    [this]{
                        handlerTask();
                    }
                );
            }
            LOG(LogLevel::INFO) << "线程池创建" << _num << "个线程成功";
        }

    public:
        // 懒汉单例模式
        // 必须使用 static 因为没有对象只能使用类名调用
        static ThreadPool<T>* getInstance() {
            // 这里加锁是必须的，但是当 _instance 已经不为 nullptr 了，而每次进入该函数还需要申请锁降低效率，所以多加一层if
            // 这里只是获取，不会影响并行的问题
            if(_instance == nullptr) {
                MutexGuard mg(_ins_mutex); 
                if(_instance == nullptr) {
                    _instance = new ThreadPool<T>;  // 这里调用构造函数 ThreadPool
                }
            }
            
            return _instance;
        }

        void start() {
            if(_is_running)
                return;
            
            _is_running = true;
            for(auto& thread : _threads) {
                thread.start();
            }
            LOG(LogLevel::INFO) << "线程池开始运行";
        }

        void stop() {
            if(!_is_running)
                return;

            _is_running = false;
            // 可能有还有线程在等待，所以需要唤醒所有等待的线程
            // 如果没有线程等待，这行代码也不影响
            if(_thread_wait_num > 0)
                _task_cond.broadcast();
        }

        void join() {
            for(auto& thread : _threads) {
                LOG(LogLevel::INFO) << "回收 " << thread.gettname() << " 线程成功";
                thread.join();
            }
        }

        // 可能有多个执行流入任务
        void enqueue(const T& task) {
            MutexGuard mg(_task_mutex);
            if(!_is_running)    // 当线程池停止时，不允许继续向任务队列中放任务
                return;
            _q.push(task);
            // 若有线程等待，则唤醒对应的线程执行任务
            if(_thread_wait_num > 0) {
                LOG(LogLevel::INFO) << "唤醒一个线程处理任务";
                _task_cond.signal();
            }
        }

        ~ThreadPool() {
            if(_instance) {
                delete _instance;
            }
        }

    private:
        std::queue<T> _q;               // 任务队列，每个任务需要提供 operator() 方法
        std::vector<Thread> _threads;   // 管理每一个线程
        unsigned int _num;              // 线程池中线程的数量
        bool _is_running;               // 线程池是否运行

        Mutex _task_mutex;
        Cond _task_cond;

        unsigned int _thread_wait_num = 0;

        // 因为在 getInstance 函数中使用，没有调用构造函数，所以普通成员还不存在，则只能使用 static 的锁
        // 静态成员变量不会在构造函数的初始化列表中调用默认构造
        // static 成员变量本质就是全局变量，在编译时就已经创建，只不过需要受类域限制
        static ThreadPool<T>* _instance;
        static Mutex _ins_mutex;   
};      

template<typename T>
ThreadPool<T>* ThreadPool<T>::_instance = nullptr;

template<typename T>
Mutex ThreadPool<T>::_ins_mutex;    // 在这里调用默认构造函数