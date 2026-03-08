#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/timerfd.h>
#include <sys/eventfd.h>
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
#include <thread>
#include <mutex>

// 日志模块的设计
#define DEBUG 0
#define INFO 1
#define ERROR 2
#define LOG_LEVEL DEBUG
#define LOG(level , format , ...) do { \
    if(level < LOG_LEVEL) break; \
    time_t tiemstacp = time(nullptr); \
    struct tm* time_info = localtime(&tiemstacp); \
    char format_time_buffer[1024] = {0}; \
    strftime(format_time_buffer , 1023 , "%Y-%m-%d %H:%M:%S" , time_info); \
    fprintf(stdout , "[%s:%s:%d] " format "\n" , format_time_buffer , __FILE__ , __LINE__ , ##__VA_ARGS__); \
} while(0)

#define DEBUG_LOG(format , ...) LOG(DEBUG , format , ##__VA_ARGS__)
#define INFO_LOG(format , ...) LOG(INFO , format , ##__VA_ARGS__)
#define ERROR_LOG(format , ...) LOG(ERROR , format , ##__VA_ARGS__)

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

class EventLoop;
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
        EventLoop* _loop;   // 不同的 Channel 可能属于不同的 EventLoop
    public:
        Channel(EventLoop* loop , int fd) :_fd(fd) , _events(0) , _revents(0) , _loop(loop) {}
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
        void EnableRead() { _events |= EPOLLIN; Update(); }
        void EnableWrite() { _events |= EPOLLOUT; Update(); }
        void DisableRead()  { _events &= ~EPOLLIN; Update(); }
        void DisableWrite() { _events &= ~EPOLLOUT; Update(); }
        void DisableALl() { _events = 0; Update(); }
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
                // 从内核中移除
                Update(EPOLL_CTL_DEL , channel);
                // 删除管理信息
                _channels.erase(channel->fd());
            }
        }
        // 开启事件监控，返回就绪的Channel列表
        void Poll(std::vector<Channel*>* active) {
            int readyfds = epoll_wait(_epfd , _events , MAX_EPOLL_SIZE , -1); // -1表示阻塞
            if(readyfds < 0) {
                if(errno == EINTR) {
                    // 被信号中断，继续等待
                    return;
                }
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
};

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
        EventLoop* _loop;   // 一个时间轮绑定一个EventLoop 
        int _timerfd;   // 定时器的文件描述符
        Channel _timerfd_channel;
    
        static int createTimerfd() {
            // CLOCK_MONOTONIC 使用系统开机的相对时间（不受手动修改系统时间的影响）
            int timerfd = timerfd_create(CLOCK_MONOTONIC , 0);
            if(timerfd < 0) {
                ERROR_LOG("create timerfd failed");
                abort();
            }
            INFO_LOG("timewheel create timerfd success: %d" , timerfd);
            /*
                struct timespec {
                    time_t tv_sec;  // 秒
                    long tv_nsec;   // 纳秒
                };

                struct itimerspec {
                    struct timespec it_interval;  // 间隔时间（周期性定时）
                    struct timespec it_value;     // 第一次超时时间
                };
            */
            struct itimerspec itimer;
            itimer.it_value.tv_sec = 1; // 第一次超时时间
            itimer.it_value.tv_nsec = 0;
            itimer.it_interval.tv_sec = 1;  // 第一次超时时间后，每次超时的间隔时间
            itimer.it_interval.tv_nsec = 0;
            timerfd_settime(timerfd , 0 , &itimer , nullptr); // flag: 0表示相对时间
            return timerfd;
        }
        // 读取 timerfd
        void readTimerfd() {
            uint64_t val = 0;
            int res = read(_timerfd , &val , sizeof(val));
            if(res < 0) {
                ERROR_LOG("read timerfd failed");
                abort();
            }
        }
        // 指针向下走一步
        void nextTick() {
            // tick指针向下走一步，并执行该位置的所有定时任务
            _tick = (_tick + 1) % _capacity;
            _wheel[_tick].clear();  
        }
        void onTime() {
            readTimerfd();
            nextTick();
        }
        // 添加一个定时任务
        void addTimerTaskInLoop(uint64_t timer_id , int delay , const TimerTaskFun& task) {
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
        void refreshTimerTaskInLoop(uint64_t timer_id) {
            if(hasTimerTask(timer_id)) {
                // 定时任务id存在才能刷新定时任务
                std::shared_ptr<TimerTask> timer_task =  _tasks[timer_id].lock();
                int pos = (_tick + timer_task->getDelayTime()) % _capacity;
                _wheel[pos].push_back(timer_task);
            }
        }
        // 删除定时任务
        void delTimerTaskInLoop(uint64_t timer_id) {
            if(hasTimerTask(timer_id)) {
                // 这一步很关键！！添加检查
                if(_tasks[timer_id].lock())
                    _tasks[timer_id].lock()->cancelTimerTask();
            }
        }
    public:
        TimerWheel(EventLoop* loop) 
            :_capacity(60) 
            ,_wheel(_capacity)
            ,_tick(0)
            ,_loop(loop)
            ,_timerfd(createTimerfd())
            ,_timerfd_channel(loop , _timerfd)
        {
            // 为定时器设置读事件就绪的回调函数
            _timerfd_channel.setReadCallback(std::bind(&TimerWheel::onTime , this)); 
            // 设置开启读事件监听 OS每隔一秒向 timefd 中写入数据，timerfd 每隔一秒就会有读事件触发
            _timerfd_channel.EnableRead();
        }
        void addTimerTask(uint64_t timer_id , int delay , const TimerTaskFun& task);
        void refreshTimerTask(uint64_t timer_id);
        void delTimerTask(uint64_t timer_id);
        /*这个接口存在线程安全问题--这个接口实际上不能被外界使用者调用，只能在模块内，在对应的EventLoop线程内执行*/
        bool hasTimerTask(uint64_t timer_id) {
            std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator iter = _tasks.find(timer_id);
            return iter != _tasks.end(); // 存在返回 true，不存在 false
        }
};

// Reactor 模块 对于事件的管理 epoll、timerfd、eventfd 的就绪事件
using Functor = std::function<void()>;
class EventLoop {
    private:
        std::thread::id _thread_id; // 一个 eventloop 对应一个线程
        int _efd; // 线程间的事件通知机制
        Channel _efd_channel;
        Poller _poll;
        TimerWheel _timer_wheel; // 构造 timerwheel 的时候会使用到 _poll 注意初始化顺序
        std::vector<Functor> _tasks; // 任务队列，对于链接的操作都必须在一个 EventLoop 中操作
        std::mutex _mutex;

        static int createEventfd() {
            int efd = eventfd(0 , EFD_CLOEXEC | EFD_NONBLOCK);
            if(efd < 0) {
                ERROR_LOG("create eventfd failed");
                abort();
            }
            INFO_LOG("eventloop create eventfd success: %d" , efd);
            return efd;
        }
        // 读取 eventfd 的事件通知
        void readEventfd() {
            uint64_t val = 0;
            ssize_t res = read(_efd , &val , sizeof(val));
            if(res < 0) {
                // EAGAIN表示当前没有数据可读，EINTR表示被信号打断
                if(errno == EAGAIN || errno == EINTR) { // 这些错误可以被原谅
                    return;
                }
                ERROR_LOG("read eventfd failed");
                abort();
            }
        }
        // 写入 eventfd 触发事件通知
        void writeEventfd() {
            uint64_t val = 1;
            ssize_t n = write(_efd , &val , sizeof(val));
            if(n < 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return;
                }
                ERROR_LOG("write eventfd failed");
                abort();
            }
        }
        // 执行任务队列中所有的任务
        void runAllTasks() {
            // 如果直接加锁执行队列中的所有任务，可能会有些任务执行处理事件过长，导致其他线程添加任务到队列中会造成竞争锁资源的阻塞
            // 因此，执行一个 vector 的交换操作，直接释放锁
            std::vector<Functor> temp;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _tasks.swap(temp);
            }
            for(auto& task : temp) task();
        }
    public:
        EventLoop()
            :_thread_id(std::this_thread::get_id())
            ,_efd(createEventfd())
            ,_efd_channel(this , _efd)
            ,_timer_wheel(this)
        {
            // 为 eventfd 设置读事件就绪的回调函数以及启动读事件回调
            _efd_channel.setReadCallback(std::bind(&EventLoop::readEventfd , this));
            _efd_channel.EnableRead();
        }
        // 启动事件循环，处理IO事件和执行任务
        void start() {
            // 1.监听事件
            std::vector<Channel*> active;
            _poll.Poll(&active);
            // 2.处理就绪事件
            for(auto& channel : active) {
                // INFO_LOG("%d 事件就绪, 处理该事件" , channel->fd());
                channel->HandlerEvent();
            }
            // 3.执行任务队列中的任务
            runAllTasks();
        }
        // 判断当前线程是否是 EventLoop 所属线程
        bool isInLoop() { return std::this_thread::get_id() == _thread_id; }
        // 断言当前线程是EventLoop所属线程
        void assertIsInLoop() { assert(std::this_thread::get_id() == _thread_id); }
        // 在当前线程执行任务，如果不是所属线程则添加到任务队列
        void runInLoop(const Functor& task) {
            if(isInLoop()) return task();
            pushInLoop(task);
        }
        // 添加任务到该线程的任务队列中
        void pushInLoop(const Functor& task) {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _tasks.push_back(task);
            }
            // 当有任务添加到任务队列中，但是没有事件就绪，导致该 EventLoop 线程阻塞在 epoll_wait 中
            // 因此，每当有新任务需要该 EventLoop 线程处理时，就需要写入 eventfd 触发事件通知
            writeEventfd();
        }
        // 更新 Poller 中的事件监听（内核层面的修改）
        void updateEvent(Channel* channel) { return _poll.UpdateEvent(channel); }
        // 从内核层面移除事件监听
        void removeEvent(Channel* channel) { return _poll.RemoveEvent(channel); }
        // 添加定时任务
        void addTimeTask(uint64_t id, uint32_t delay, const TimerTaskFun& task) { return _timer_wheel.addTimerTask(id , delay , task); }
        // 刷新定时任务（延迟其超时时间）
        void refreshTimeTask(uint64_t id) { return _timer_wheel.refreshTimerTask(id); }
        // 删除定时任务
        void delTimeTask(uint64_t id) { return _timer_wheel.delTimerTask(id); }
        // 是否存在定时任务
        bool hasTimeTask(uint64_t id) { return _timer_wheel.hasTimerTask(id); }
};

// 更新或移除(通过 EventLoop 中的 epoll模块对事件进行内核级的更新和移除)
void Channel::Remove() { return _loop->removeEvent(this); }
void Channel::Update() { return _loop->updateEvent(this); }

void TimerWheel::addTimerTask(uint64_t timer_id , int delay , const TimerTaskFun& task) { 
    return _loop->runInLoop(std::bind(&TimerWheel::addTimerTaskInLoop , this , timer_id , delay , task));
}
void TimerWheel::refreshTimerTask(uint64_t timer_id) {
    return _loop->runInLoop(std::bind(&TimerWheel::refreshTimerTaskInLoop , this , timer_id));
}
void TimerWheel::delTimerTask(uint64_t timer_id) {
    return _loop->runInLoop(std::bind(&TimerWheel::delTimerTaskInLoop , this , timer_id));
}       

// 这是一个对链接管理的类
class Connection;
typedef enum {
    DISCONNECTED  = 0,
    DISCONNECTING = 1,
    CONNECTING    = 2,
    CONNECTED     = 3
} ConnStatus;
// 这四个回调函数是由组件使用者设置的
using MessageCallback = std::function<void(const std::shared_ptr<Connection>& , Buffer*)>;
using ClosedCallback = std::function<void(const std::shared_ptr<Connection>&)>;
using AnyEventCallback = std::function<void(const std::shared_ptr<Connection>&)>;
using ConnectedCallback = std::function<void(const std::shared_ptr<Connection>&)>;
class Connection : public std::enable_shared_from_this<Connection> {
    private:
        uint64_t _conn_id;  // 连接的id
        ConnStatus _status; // 连接的状态
        bool _enable_inactive_release; // 是否开启当前连接的非活跃销毁释放
        int _sockfd;    // 文件描述符
        EventLoop* _loop; // 每个连接绑定一个 EventLoop
        Channel _channel;  // 对于 _sockfd 的用户层事件管理 channel 依赖 _sockfd 和 _loop 注意构造顺序
        Socket _socket; //  对套接字的操作
        Buffer _in_buffer;  // 用户层输入缓冲区
        Buffer _out_buffer; // 用户层输出缓冲区
        Any _context; // 协议处理的上下文
        MessageCallback _message_callback; // 收到数据后的回调
        ClosedCallback _closed_callback;    // 连接关闭后的回调
        AnyEventCallback _any_event_callback;   // 任意事件后的回调
        ConnectedCallback _connected_callback;  // 连接建立成功后的回调
        ClosedCallback _manager_release; // TcpServer 类将其 Connection 从管理容器中移除的回调
    private:
        // 处理读事件就绪的回调函数
        void HandlerRead() {
            char buffer[65536] = {0};
            ssize_t res = _socket.nonBlockRecv(buffer , 65535);
            if(res < 0) {
                // 读取错误，检查是否有待处理数据，若有待处理数据，则处理完释放连接
                return ShutdownInLoop();
            }
            // 读取成功，写入到输入缓冲区
            _in_buffer.writeAndPush(buffer , res);
            // 若输入缓冲区有数据则调用用户的回调函数处理业务
            if(_in_buffer.getReadableSize() > 0)
                _message_callback(shared_from_this() , &_in_buffer);
        }
        // 处理写事件就绪的回调函数
        void HandlerWrite() {
            ssize_t res = _socket.nonBlockSend(_out_buffer.readPosition() , _out_buffer.getReadableSize());
            if(res < 0) {
                // 写入错误，检查是否输入缓冲区是否还有数据待处理，调用用户回调函数处理
                if(_in_buffer.getReadableSize() > 0)
                    _message_callback(shared_from_this() , &_in_buffer);
                // 释放连接
                return ReleaseInLoop();
            }
            // 写入成功后，移动实际的写入位置
            _out_buffer.moveReadOffset(res);
            // 若写入完毕，则关闭写事件监控
            if(_out_buffer.getReadableSize() == 0) {
                // 关闭写事件监听
                _channel.DisableWrite();
                // 若当前连接为半关闭状态，则发送完本次数据，应该释放连接
                if(_status == DISCONNECTING)
                    return ReleaseInLoop();
            } 
        }
        // 处理挂断事件的回调函数
        void HandlerClose() {
            _status = DISCONNECTING;
            // 一旦连接挂断，套接字则不能写入数据，因此检查一下输入缓冲区是否有待处理数据
            if(_in_buffer.getReadableSize() > 0)
                _message_callback(shared_from_this() , &_in_buffer);
            return ReleaseInLoop();
        }
        // 处理错误事件的回调函数
        void HandlerError() {
            return HandlerClose();
        }
        // 处理任意事件的回调函数
        void HandlerEvent() {
            // 连接为半关闭或关闭状态直接返回
            if(_status == DISCONNECTING || _status == DISCONNECTED)
                return;
            // 刷新定时任务的活跃度
            if(_enable_inactive_release) {
                _loop->refreshTimeTask(_conn_id);
            }
            // 调用组件使用者的任意事件回调
            if(_any_event_callback)
                _any_event_callback(shared_from_this());
        }
        // 释放连接
        void ReleaseInLoop() {
            // 1.修改连接状态为关闭状态
            _status = DISCONNECTED;
            // 2.删除epoll内核中的管理
            _channel.Remove();
            // 3.关闭文件描述符
            _socket.close();
            // 4.如果当前定时器队列中还有定时销毁任务，则取消任务
            if(_loop->hasTimeTask(_conn_id)) CancelInactiveReleaseInLoop();
            // 5.调用使用者的关闭连接回调函数
            if(_closed_callback) _closed_callback(shared_from_this());
            // 6.调用 TcpServer 类将其 Connection 从管理容器中移除的回调
            if(_manager_release) _manager_release(shared_from_this());
        }
        // 为连接做初始化工作
        void EstablishedInLoop() {
            assert(_status == CONNECTING);
            // 1.修改连接状态为建立完毕状态
            _status = CONNECTED;
            // 2.开启读事件监控
            _channel.EnableRead();
            // 3.调用使用者的连接建立完成的回调
            if(_connected_callback) _connected_callback(shared_from_this());
        }
        // 向连接的发送接口
        void SendInLoop(Buffer buf) {
            if(_status == DISCONNECTED) return;
            // 1.写入到输入缓冲区
            _out_buffer.writeAndPush(buf.readPosition() , buf.getReadableSize());
            // 2.开启写事件监控
            if(_channel.IsMoinitorWrite() == false)
                _channel.EnableWrite();
        }
        // 关闭连接，在彻底释放连接前，检查是否还有数据待处理
        void ShutdownInLoop() {
            // 1.修改连接状态为半关闭状态
            _status = DISCONNECTING;
            // 2.检查输入缓冲区
            if(_in_buffer.getReadableSize() > 0)
                _message_callback(shared_from_this() , &_in_buffer);
            // 3.检查输入缓冲区
            if(_out_buffer.getReadableSize() > 0)
                if(_channel.IsMoinitorWrite() == false)
                    _channel.EnableWrite();
            // 4.释放连接
            if(_out_buffer.getReadableSize() == 0)
                ReleaseInLoop();
        }
        // 启动定时销毁连接任务
        void EnableInactiveReleaseInLoop(int sec) {
            _enable_inactive_release = true;
            // 启动若已存在，则刷新，不存在，则创建
            _loop->addTimeTask(_conn_id , sec , std::bind(&Connection::ReleaseInLoop , this));
        }
        // 取消定时销毁连接任务
        void CancelInactiveReleaseInLoop() {
            DEBUG_LOG("CancelInactiveReleaseInLoop");
            _enable_inactive_release = false;
            if(_loop->hasTimeTask(_conn_id)) {
                return _loop->delTimeTask(_conn_id);
            }
        }
        // 更换协议上下文和处理回调函数
        void UpgradeInLoop(
            const Any& context, 
            const ConnectedCallback& connected_cb,
            const MessageCallback& message_cb,
            const ClosedCallback& closed_cb,
            const AnyEventCallback& any_event_cb
        ) {
            _context = context;
            _message_callback = message_cb;
            _closed_callback = closed_cb;
            _any_event_callback = any_event_cb;
            _connected_callback = connected_cb;
        }
    public:
        Connection(uint64_t id , EventLoop* loop , int fd)
            :_conn_id(id)
            ,_status(CONNECTING)
            ,_enable_inactive_release(false)
            ,_sockfd(fd)
            ,_loop(loop)
            ,_channel(loop , fd)
            ,_socket(fd)
        {
            // 为 channel 设置事件就绪回调
            _channel.setReadCallback(std::bind(&Connection::HandlerRead , this));
            _channel.setWriteCallback(std::bind(&Connection::HandlerWrite , this));
            _channel.setCLoseCallback(std::bind(&Connection::HandlerClose , this));
            _channel.setErrorCallback(std::bind(&Connection::HandlerError , this));
            _channel.setEventCallback(std::bind(&Connection::HandlerEvent , this));
            // 开启读事件监控不能在构造函数中，需要在外边的定时器任务开启后，在开启读事件的监听
        }
        ~Connection() { DEBUG_LOG("Release Conncection: %p" , this); }
        int Fd() { return _sockfd; }
        uint64_t Id() { return _conn_id; }
        bool Connected() { return _status == CONNECTED; }
        void SetContext(const Any& context) { _context = context; }
        Any* GetContext() { return &_context; }
        void SetConnectedCallBack(const ConnectedCallback& cb) { _connected_callback = cb; }
        void SetMessageCallBack(const MessageCallback& cb) { _message_callback = cb; }
        void SetClosedCallBack(const ClosedCallback& cb) { _closed_callback = cb; }
        void SetAnyEventCallBack(const AnyEventCallback& cb) { _any_event_callback = cb; }
        void SetServerClosedCallBack(const ClosedCallback& cb) { _manager_release = cb; }
        void Established() { _loop->runInLoop(std::bind(&Connection::EstablishedInLoop , this)); }
        void Send(const char* data , size_t len) {
            //外界传入的data，可能是个临时的空间，我们现在只是把发送操作压入了任务池，有可能并没有被立即执行
            //因此有可能执行的时候，data指向的空间有可能已经被释放了。
            Buffer buf;
            buf.writeAndPush(data , len);
            _loop->runInLoop(std::bind(&Connection::SendInLoop , this , buf));
        }
        void Shutdown() { _loop->runInLoop(std::bind(&Connection::ShutdownInLoop , this)); }
        void EnableInactiveRelease(int sec) { _loop->runInLoop(std::bind(&Connection::EnableInactiveReleaseInLoop , this , sec)); }
        void CancelInactiveRelease() { _loop->runInLoop(std::bind(&Connection::CancelInactiveReleaseInLoop , this)); }
        void Upgrade(
            const Any& context, 
            const ConnectedCallback& connected_cb,
            const MessageCallback& message_cb,
            const ClosedCallback& closed_cb,
            const AnyEventCallback& any_event_cb
        ) {
            // 这个上下文切换的函数应该是立即执行切换的，必须在本线程中调用
            _loop->assertIsInLoop();
            _loop->runInLoop(std::bind(&Connection::UpgradeInLoop , this , context , connected_cb , message_cb , closed_cb , any_event_cb));
        }
};