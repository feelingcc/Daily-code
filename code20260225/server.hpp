#pragma once
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <algorithm>

// 日志模块的设计
#define DEBUG 0
#define INFO 1
#define ERROR 2
#define LOG_LEVEL DEBUG
#define LOG(level , format , ...) do { \
    if(level < LOG_LEVEL) break; \
    time_t tiemstacp = time(nullptr); \
    struct tm* time_info = localtime(&tiemstacp); \
    char buffer[1024] = {0}; \
    strftime(buffer , 1023 , "%Y-%m-%d %H:%M:%S" , time_info); \
    fprintf(stdout , "[%s:%s:%d] " format "\n" , buffer , __FILE__ , __LINE__ , ##__VA_ARGS__); \
} while(0)

#define DEBUG_LOG(format , ...) LOG(DEBUG , format , ##__VA_ARGS__)
#define INFO_LOG(format , ...) LOG(INFO , format , ##__VA_ARGS__)
#define ERROR_LOG(format , ...) LOG(ERROR , format , ##__VA_ARGS__)

// 时间轮与定时任务的设计
using TimerTaskFun = std::function<void()>;
using ReleaseTaskFun = std::function<void()>;
class TimerTask {
    private:
        uint64_t _timer_id; // 定时任务id
        int _timeout;   // 延迟时间
        bool _cancel;   // 是否执行定时任务
        TimerTaskFun _task;    // 定时任务
        ReleaseTaskFun _release_task; // 删除当前任务在时间轮管理容器中的回调函数
    public:
        TimerTask(uint64_t timer_id , int timeout , const TimerTaskFun& task)
            :_timer_id(timer_id)
            ,_timeout(timeout)
            ,_task(task)
            ,_cancel(true)
        {}
        // 获取定时任务的延迟时间
        int getDelayTime() { return _timeout; }
        // 设置删除当前任务在时间轮管理容器中的回调函数
        void setReleaseTask(const ReleaseTaskFun& release_task) { _release_task = release_task; }
        // 取消定时任务的执行
        void cancelTimerTask() { _cancel = false; } 
        // 析构函数中执行定时任务
        ~TimerTask() {
            if(_cancel) _task();
            // 任务执行完毕后，将其从时间轮的哈希表中移除
            _release_task();
        }
};

class TimerWheel{
    private:
        int _capacity; // 时间轮的大小
        std::vector<std::vector<std::shared_ptr<TimerTask>>> _wheel;    // 时间轮
        int _tick; // 指针
        std::unordered_map<uint64_t , std::weak_ptr<TimerTask>> _tasks; // 管理所有的任务

        bool hasTimerTask(uint64_t timer_id) {
            std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator iter = _tasks.find(timer_id);
            return iter != _tasks.end(); // 存在返回 true，不存在 false
        }
    public:
        TimerWheel() 
            :_capacity(60) , _wheel(_capacity) , _tick(0)
        {}
        // 添加一个定时任务
        void addTimerTask(uint64_t timer_id , int delay , const TimerTaskFun& task) {
            if(hasTimerTask(timer_id)) {
                // 若当前定时任务存在，就刷新一下定时任务
                return refreshTimerTask(timer_id);
            }
            // 新增定时任务逻辑
            std::shared_ptr<TimerTask> timer_task = std::make_shared<TimerTask>(timer_id , delay , task);
            // 设置定时任务执行结束后在时间轮管理容器中删除的回调函数
            timer_task->setReleaseTask([& , this](){
                std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator iter = _tasks.find(timer_id);
                if(iter != _tasks.end()) {
                    _tasks.erase(iter);
                }
            });
            int pos = (_tick + delay) % _capacity;
            _wheel[pos].push_back(timer_task);
            // 将新添加的定时任务管理起来
            _tasks[timer_id] = timer_task;
        }
        // 刷新定时任务
        void refreshTimerTask(uint64_t timer_id) {
            if(hasTimerTask(timer_id)) {
                // 定时任务id存在才能刷新定时任务
                std::shared_ptr<TimerTask> timer_task =  _tasks[timer_id].lock();
                int pos = (_tick + timer_task->getDelayTime()) % _capacity;
                _wheel[pos].push_back(timer_task);
            }
        }
        // 删除定时任务
        void delTimerTask(uint64_t timer_id) {
            if(hasTimerTask(timer_id)) {
                _tasks[timer_id].lock()->cancelTimerTask();
            }
        }
        // 指针向下走一步
        void nextTick() {
            // tick指针向下走一步，并执行该位置的所有定时任务
            _tick = (_tick + 1) % _capacity;
            _wheel[_tick].clear();  
        }
};

// any类的设计
class Any{
    private:
        class placeholder{
            public:
                virtual ~placeholder() {}
                virtual const std::type_info& type() = 0;
                virtual placeholder* clone() = 0;
        };
        template<typename T>
        class holder : public placeholder{
            private:
                T _val;
            public:
                holder(const T& val) :_val(val) {}
                // 获取当前 T 的类型
                virtual const std::type_info& type() override { return typeid(_val); }
                // 克隆一个子类对象由父类指针指向
                virtual placeholder* clone() override {
                    return new holder<T>(_val);
                }
                T* getVal() { return &_val; }
        };
        placeholder* _context;
    public:
        void swap(Any& other) { std::swap(_context , other._context); }
        Any() :_context(nullptr) {}
        template<typename T>
        Any(const T& val) :_context(new holder<T>(val)) {}
        Any(const Any& other) :_context(other._context ? other._context->clone() : nullptr) {}
        Any& operator=(Any other) {
            swap(other);
            return *this;
        }
        template<typename T>
        Any& operator=(const T& val) {
            Any(val).swap(*this);
            return *this;
        }
        ~Any() { delete _context; }
        template<typename T>
        T* get() {
            // 获取的类型必须和保存的类型数据一致
            assert(_context);
            assert(typeid(T) == _context->type());
            return dynamic_cast<holder<T>*>(_context)->getVal(); // dynamic_cast 在模板场景下要求完全匹配，不只是父类指向子类就行
        }
};

// Buffer模块的设计
#define DEFAULT_BUFFER_SIZE 1024
class Buffer{
    private:
        std::vector<char> _buffer;
        uint64_t _rindex;
        uint64_t _windex;
    public:
        Buffer() :_buffer(DEFAULT_BUFFER_SIZE) , _rindex(0) , _windex(0) {}
        Buffer(const Buffer& buf) :_buffer(buf._buffer.begin() , buf._buffer.end()) , _rindex(buf._rindex) , _windex(buf._windex) {}
        ~Buffer() {}
        // 返回当前写入位置的起始地址
        char* writePosition() { return _buffer.data() + _windex; }
        // 返回当前读取位置的起始地址
        char* readPosition() { return _buffer.data() + _rindex; }
        // 获取尾部空闲空间的大小
        uint64_t getTailIdleSize() { return _buffer.size() - _windex; }
        // 获取头部空闲空间的大小
        uint64_t getHeadIdleSize() { return _rindex; }
        // 获取可读空间大小
        uint64_t getReadableSize() { return _windex - _rindex; }
        // 移动读偏移
        void moveReadOffset(uint64_t len) { assert(len <= getReadableSize()); _rindex += len; }
        // 移动写偏移
        void moveWriteOffset(uint64_t len) { assert(_windex + len <= _buffer.size()); _windex += len; }
        // 确保有足够空间写入指定长度数据
        void EnsureWriteSpace(uint64_t len) {
            if(len <= getTailIdleSize()) {
                return;
            } else if(len <= getHeadIdleSize() + getTailIdleSize()) {
                uint64_t current_readable_size = getReadableSize();
                std::copy(readPosition() , writePosition() , _buffer.data());
                _rindex = 0;
                _windex = current_readable_size;
            } else {
                // std::cout << "扩容：" << _windex + len << std::endl;
                _buffer.resize(_windex + len);
            }
        }
        // 写入数据（不移动写偏移）
        void write(const void* data , uint64_t len) {
            if(len == 0) return;
            EnsureWriteSpace(len);
            const char* d = (const char*)data;
            std::copy(d , d + len , writePosition());
        }
        // 写入数据并移动写偏移
        void writeAndPush(const void* data , uint64_t len) {
            write(data , len);
            moveWriteOffset(len);
        }
        // 写入字符串（不移动写偏移）
        void writeString(const std::string& data) {
            write(data.c_str() , data.size());
        }
        // 写入字符串并移动写偏移
        void writeStringAndPush(const std::string& data) {
            writeString(data);
            moveWriteOffset(data.size());
        }
        // 读取数据到指定缓冲区（不移动读偏移）
        void read(void* buf , uint64_t len) {
            assert(len <= getReadableSize());
            std::copy(readPosition(), readPosition() + len , (char*)buf);
        }
        // 读取数据到指定缓冲区并移动读偏移
        void readAndPop(void *buf , uint64_t len) {
            read(buf , len);
            moveReadOffset(len);
        }
        // 读取字符串到stirng中（不移动读偏移）
        std::string readAsString(uint64_t len) {
            std::string res(len , 0);
            read(res.data() , len);
            return res;
        }
        // 读取字符串到string中并移动读偏移
        std::string readAsStringAndPop(uint64_t len) {
            std::string res = readAsString(len);
            moveReadOffset(len);
            return res;
        }
        // 寻找回车换行符
        char* findCRLF() {
            char* pos = std::find(readPosition() , writePosition() , '\n');
            if(pos != writePosition())
                return pos;
            return nullptr;
        }
        // 读取一行数据不移动读偏移
        std::string getLine() {
            char* pos = findCRLF();
            if(pos) {
                return readAsString(pos + 1 - readPosition());    // 将换行符也读取出来
            }
            return "";
        }
        // 读取一行数据并移动读偏移
        std::string getLineAndPop() {
            std::string res = getLine();
            moveReadOffset(res.size());
            return res;
        }
        void clear() { _rindex = _windex = 0; }
};

// 套接字模块的设计