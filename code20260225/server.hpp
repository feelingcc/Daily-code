#pragma once
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/epoll.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
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
#define MAX_BACKLOG 1024
class Socket{
    private:
        int _sockfd;
    public:
        Socket() :_sockfd(-1) {}
        Socket(int sockfd) :_sockfd(sockfd) {}
        ~Socket() { close(); }
        void close() {
            if(_sockfd > 0) {
                ::close(_sockfd);
                _sockfd = -1;
            }
        }
        int fd() { return _sockfd; }
        bool create() {
            _sockfd = ::socket(AF_INET , SOCK_STREAM , 0);
            if(_sockfd < 0) {
                ERROR_LOG("listen socket create failed");
                return false;
            }
            INFO_LOG("listen socket create success: %d" , _sockfd);
            return true;
        }
        bool bind(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            memset(&addr , 0 , sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = ::bind(_sockfd , (struct sockaddr*)&(addr) , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("listen socket bind failed");
                return false;
            }
            INFO_LOG("listen socket bind success: %d" , _sockfd);
            return true;
        }
        bool listen(int backlog = MAX_BACKLOG) {
            int res = ::listen(_sockfd , backlog);
            if(res < 0) {
                ERROR_LOG("listen socket listen failed");
                return false;
            }
            INFO_LOG("listen socket listen success: %d" , _sockfd);
            return true;
        }
        int accept() {
            int acceptfd = ::accept(_sockfd , nullptr , nullptr);
            if(acceptfd < 0) {
                ERROR_LOG("accept failed");
                return -1;
            }
            INFO_LOG("accept success: %d" , acceptfd);
            return acceptfd;
        }
        bool connect(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            memset(&addr , 0 , sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = ::connect(_sockfd , (struct sockaddr*)&(addr) , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("conncet server failed");
                return false;
            }
            INFO_LOG("conncet server success");
            return true;
        }
        ssize_t recv(void* buf , size_t len , int flags = 0) {
            ssize_t res = ::recv(_sockfd , buf , len , flags);
            if(res <= 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return 0; // 这两种错误可以被原谅
                }
                // 读取错误或对方关闭连接都算错误
                ERROR_LOG("sockfd: %d recv failed" , _sockfd);
                return -1;
            }
            return res;
        }
        ssize_t nonBlockRecv(void* buf , size_t len) {
            return recv(buf , len , MSG_DONTWAIT); // MSG_DONTWAIT 表示当前接收为非阻塞
        }
        ssize_t send(const void* buf , size_t len , int flags = 0) {
            ssize_t res = ::send(_sockfd , buf , len , flags);
            if(res < 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return 0;   // 不算错误
                }
                ERROR_LOG("sockfd: %d send failed" , _sockfd);
                return -1;
            }
            return res;
        }
        ssize_t nonBlockSend(const void* buf , size_t len) {
            if(len == 0) return 0;
            return send(buf ,len , MSG_DONTWAIT);
        }
        bool createServer(uint64_t port , const std::string& ip = "0.0.0.0" , int block_flag = false) {
            if(!create()) return false;
            if(block_flag) setNonBlock();
            setReuseAddress();
            if(!bind(ip , port)) return false;
            if(!listen()) return false;  
            return true;  
        }
        bool createClient(uint16_t port , const std::string& ip) {
            if(!create()) return false;
            if(!connect(ip , port)) return false;
            return true;
        }
        bool setNonBlock() {
            int flag = fcntl(_sockfd , F_GETFL);
            if(flag < 0) {
                ERROR_LOG("fcntl failed");
                return false;
            }
            fcntl(_sockfd , F_SETFL , flag | O_NONBLOCK);
            return true;
        }
        void setReuseAddress() {
            int opt = 1;
            int res = setsockopt(_sockfd , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt));
            if(res < 0) {
                ERROR_LOG("set reuse address failed");
                return;
            }
            res = setsockopt(_sockfd , SOL_SOCKET , SO_REUSEPORT , &opt , sizeof(opt));
            if(res < 0) {
                ERROR_LOG("set reuse address failed");
                return;
            }
        }
};

// 用户层对监听和就绪的事件的管理
using event_callback = std::function<void()>;
class Channel{
    private:
        int _fd;
        uint32_t _events; // 监控的事件
        uint32_t _revents;// 就绪的事件
        // 对应事件就绪后，调用相应的回调函数
        event_callback readCallback;  // 读事件就绪的回调
        event_callback writeCallback; // 写事件就绪的回调
        event_callback closeCallback; // 连接挂断后的回调
        event_callback errorCallback; // 错误后的回调
        event_callback eventCallback; // 任意事件的回调
    public:
        Channel(int fd) :_fd(fd) , _events(0) , _revents(0) {}
        // get/set方法
        int fd() { return _fd; }
        int events() { return _events; }
        int revents() { return _revents; }
        void setRevents(uint32_t revents) { _revents = revents; }
        // 设置回调方法
        void setReadCallback(const event_callback& cb) { readCallback = cb; }
        void setWriteCallback(const event_callback& cb) { writeCallback = cb; }
        void setErrorCallback(const event_callback& cb) { errorCallback = cb; }
        void setCLoseCallback(const event_callback& cb) { closeCallback = cb; }
        void setEventCallback(const event_callback& cb) { eventCallback = cb; }
        // 查询是否监控了可读与可写事件
        bool IsMonitorReadable() { return _events & EPOLLIN; }
        bool IsMoinitorWrite() { return _events & EPOLLOUT; }
        // 启动或关闭事件监控
        void EnableRead() { _events |= EPOLLIN; }
        void EnableWrite() { _events |= EPOLLOUT; }
        void DisableRead()  { _events &= ~EPOLLIN; }
        void DisableWrite() { _events &= ~EPOLLOUT; }
        void DisableALl() { _events = 0; }
        // 更新或移除(通过epoll模块对事件进行内核级的更新和移除)
        void Remove();
        void Update();
        // 事件处理函数，根据触发的事件调用相应的回调
        void HandlerEvent() {
            // EPOLLRDHUP：对方正常关闭连接; EPOLLHUP：连接异常断开（对端异常奔溃）
            if(_revents | EPOLLIN || _revents | EPOLLRDHUP || _revents | EPOLLPRI) {
                // 刷新连接的活跃度，放在读回调之前是因为可能读错误或连接断开释放连接，在调用任意事件回调就会奔溃
                if(eventCallback) eventCallback(); 
                if(readCallback) readCallback();
            }
            // 有可能释放连接的操作，一次只处理一个
            if(_revents | EPOLLOUT) {
                if(eventCallback) eventCallback();
                if(writeCallback) writeCallback();
            } else if(_revents | EPOLLERR) {
                if(errorCallback) errorCallback();
            } else if(_revents | EPOLLHUP) {
                if(closeCallback) closeCallback();
            }
        }
};

// 对 epoll 的封装
#define MAX_EPOLL_SIZE 1024
class Poller{
    private:
        int _epfd;
        struct epoll_event _events[MAX_EPOLL_SIZE];    // 就绪的事件队列
        std::unordered_map<int , Channel*> _channels;    // epoll 管理所有的用户层 Channel，描述符与Channel的映射
        // 内部方法，执行实际的 epoll_ctl 操作
        void Update(int op, Channel* channel) {
            struct epoll_event ev;
            memset(&ev , 0 , sizeof(ev));
            ev.events = channel->events();
            ev.data.fd = channel->fd();
            int res = epoll_ctl(_epfd , op , channel->fd() , &ev);
            if(res < 0) {
                ERROR_LOG("epoll ctl failed");
                abort();
            }
            return;
        }
        // 判断是否存在 Channel
        bool hasChannel(Channel* channel) {
            std::unordered_map<int , Channel*>::iterator iter = _channels.find(channel->fd());
            return iter != _channels.end();  // 存在返回 true，不存在返回 false
        }
    public:
        Poller() {
            _epfd = epoll_create(MAX_EPOLL_SIZE);
            if(_epfd < 0) {
                ERROR_LOG("epoll create failed");
                abort();
            }
            INFO_LOG("epoll create success: %d" , _epfd);
        }
        ~Poller() {
            if(_epfd > 0) {
                ::close(_epfd);
                _epfd = -1;
            }
        }
         // 添加或修改事件监控
        void UpdateEvent(Channel* channel) {
            if(hasChannel(channel))
                return Update(EPOLL_CTL_MOD , channel);
            Update(EPOLL_CTL_ADD , channel);
            // 添加管理信息
            _channels[channel->fd()] = channel;
        }
        // 移除事件监控
        void RemoveEvent(Channel* channel) {
            if(hasChannel(channel)) {
                return Update(EPOLL_CTL_DEL , channel);
            }
        }
        // 开启事件监控，返回就绪的Channel列表
        void Poll(std::vector<Channel*>* active) {
            INFO_LOG("开启事件监控 epoll_wait");
            while(true) {
                int readyfds = epoll_wait(_epfd , _events , MAX_EPOLL_SIZE , -1); // -1表示阻塞
                if(readyfds < 0) {
                    ERROR_LOG("epoll wait failed: %s" , strerror(errno));
                    abort();
                }
                for(int i = 0; i < readyfds; i++) {
                    auto iter = _channels.find(_events[i].data.fd);
                    assert(iter != _channels.end()); // 一定存在，否则管理信息出现问题
                    // 更新用户层 Channel 的就绪事件
                    iter->second->setRevents(_events[i].events);
                    active->push_back(iter->second);    // 添加到就绪队列中
                }
            }
        }
};

// Reactor 模块
class EventLoop {
    private:

    public:
        
};