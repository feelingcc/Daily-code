#pragma once
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

// 简易日志的设计
#define INFO  0
#define DEBUG 1
#define ERROR 2
#define CURRENT_LOG_LEVEL DEBUG
// ##__VA_ARGS__表示0个或多个参数
#define LOG(level , format , ...) do{ \
    if(level < CURRENT_LOG_LEVEL) break; \
    time_t timestamp = time(nullptr); \
    struct tm* local = localtime(&timestamp); \
    char time_format[1024] = {0}; \
    strftime(time_format , sizeof(time_format) , "%Y-%m-%d %H:%M:%S" , local); \
    fprintf(stdout , "[%s:%s:%d] " format "\n" , time_format , __FILE__ , __LINE__ , ##__VA_ARGS__); \
} while(0)
#define INFO_LOG(format , ...) LOG(INFO , format , ##__VA_ARGS__)
#define DEBUG_LOG(format , ...) LOG(DEBUG , format , ##__VA_ARGS__)
#define ERROR_LOG(format , ...) LOG(ERROR , format , ##__VA_ARGS__)

// 缓冲区类的设计
#define BUFFER_DEFAULT_SIZE 1024
class Buffer{
    public:
        Buffer() :_buffer(BUFFER_DEFAULT_SIZE) , _reader_idx(0) , _writer_idx(0) {}  // 初始化缓冲区，设置读写偏移为0，分配默认大小空间
        // 拷贝构造函数
        Buffer(const Buffer& buf) 
            :_buffer(buf._buffer.begin() , buf._buffer.end()) 
            ,_reader_idx(buf._reader_idx)
            ,_writer_idx(buf._writer_idx)
        {}   
        char* Begin() { return &*_buffer.begin(); } // 返回缓冲区的起始地址
        char* WritePosition() { return Begin() + _writer_idx; }     // 返回当前写入的起始地址
        char* ReadPosition() { return Begin() + _reader_idx; }      // 返回当前读取的起始地址
        uint64_t TailIdleSize() { return _buffer.size() - _writer_idx; } // 获取尾部空闲空间的大小
        uint64_t HeadIdleSize() { return _reader_idx; } // 获取头部空闲空间的大小
        uint64_t ReadableSize() { return _writer_idx - _reader_idx; } // 获取可读空间大小
        void MoveReadOffset(uint64_t len) {
            assert(len <= ReadableSize());
            _reader_idx += len;
        }
        void MoveWriteOffset(uint64_t len) {
            assert(len <= TailIdleSize());
            _writer_idx += len;
        }
        void EnsureWriteSpace(uint64_t len) { // 确保有足够空间写入指定长度数据（自动移动数据或扩容）
            if(len <= TailIdleSize()) {
                // 尾部空闲空间足够
                return;
            } else if(len <= TailIdleSize() + HeadIdleSize()) {
                // 尾部空闲空间 + 头部空闲空间足够，数据整体向前移动，无需扩容
                uint64_t readable_size = ReadableSize();
                std::copy(ReadPosition() , WritePosition() , Begin());
                _reader_idx = 0;
                _writer_idx = readable_size;
            } else {
                // 扩容
                DEBUG_LOG("resize %ld" , _writer_idx + len);
                _buffer.resize(_writer_idx + len);
            }
        } 
        void Write(const void* data , uint64_t len) { // 写入数据（不移动写偏移）
            if(len == 0) return;
            EnsureWriteSpace(len);
            const char* d = (const char*)data;
            std::copy(d , d + len , WritePosition());
        } 
        void WriteAndPush(const void* data , uint64_t len) { // 写入数据并移动写偏移
            Write(data , len);
            MoveWriteOffset(len);
        } 
        void WriteString(const std::string& data) { // 写入字符串（不移动写偏移）
            Write(data.c_str() , data.size());
        }    
        void WriteStringAndPush(const std::string& data) { // 写入字符串并移动写偏移
            WriteString(data);
            MoveWriteOffset(data.size());
        }   
        void Read(void* buf , uint64_t len) { // 读取数据到指定缓冲区（不移动读偏移）
            assert(len <= ReadableSize());
            std::copy(ReadPosition() , ReadPosition() + len , (char*)buf);
        }  
        void ReadAndPop(void* buf , uint64_t len) { // 读取数据到指定缓冲区并移动读偏移
            Read(buf , len);
            MoveReadOffset(len);
        } 
        std::string ReadAsString(uint64_t len) { // 读取数据到string中（不移动读偏移）
            assert(len <= ReadableSize());
            std::string res;
            res.resize(len);
            Read(&res[0] , len);
            return res;
        }  
        std::string ReadAsStringAndPop(uint64_t len) { // 读取数据到string中并移动读偏移
            std::string res = ReadAsString(len);
            MoveReadOffset(len);
            return res;
        } 
        char* FindCRLF() {  // 寻找换行回车符
            return std::find(ReadPosition() , WritePosition() , '\n');
        } 
        std::string GetLine() { // 获取一行数据（包括换行符，不移动读偏移）
            char* res = FindCRLF();
            if(res != WritePosition()) {
                // 找到了换行符
                return ReadAsString(res + 1 - ReadPosition());
            }
            return "";
        }   
        std::string GetLineAndPop() { // 获取一行数据（包括换行符）并移动读偏移
            std::string res = GetLine();
            MoveReadOffset(res.size());
            return res;
        } 
        void Clear() { _reader_idx = _writer_idx = 0; }     // 清理接口
    private:
        std::vector<char> _buffer; 
        uint64_t _reader_idx;  
        uint64_t _writer_idx;
};

// 套接字类的设计
#define DEFAULT_BACKLOG 1024
class Socket{
    public:
        Socket() :_sockfd(-1) {}
        Socket(int fd) :_sockfd(fd) {}
        ~Socket() { Close(); }
        int Fd() { return _sockfd; }
        bool Create() {
            _sockfd = socket(AF_INET , SOCK_STREAM , 0);
            if(_sockfd < 0) {
                ERROR_LOG("create socket failed");
                return false;
            }
            return true;
        }
        bool Bind(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = bind(_sockfd , (struct sockaddr*)&addr , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("bind server failed");
                return false;
            }
            return true;
        }
        bool Listen(int backlog = DEFAULT_BACKLOG) {
            int res = listen(_sockfd , backlog);
            if(res < 0) {
                ERROR_LOG("listen server failed");
                return false;
            }
            return true;
        }
        int Accept() {
            int newfd = accept(_sockfd , nullptr , nullptr);
            if(newfd < 0) {
                ERROR_LOG("socket accept failed");
                return -1;
            }
            return newfd;
        }
        bool Connect(const std::string& ip , uint16_t port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            int res = connect(_sockfd , (struct sockaddr*)&addr , sizeof(addr));
            if(res < 0) {
                ERROR_LOG("connect server failed");
                return false;
            }
            return true;
        }
        ssize_t Recv(void* buf , size_t len , int flags = 0) {
            ssize_t n = recv(_sockfd , buf , len , flags);
            if(n <= 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return 0;   // 不算错误
                }
                // 读取错误或对方关闭连接都算错误
                ERROR_LOG("socket recv failed");
                return -1;
            }
            return n;
        }
        ssize_t NonBlockRecv(void* buf , size_t len) {
            return Recv(buf , len , MSG_DONTWAIT); // MSG_DONTWAIT 表示当前接收为非阻塞
        }
        ssize_t Send(const void* buf , size_t len , int flags = 0) {
            ssize_t res = send(_sockfd , buf , len , flags);
            if(res <= 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return 0;   // 不算错误
                }
                // 读取错误或对方关闭连接都算错误
                ERROR_LOG("socket send failed");
                return -1;
            }
            return res;
        }
        ssize_t NonBlockSend(void* buf , size_t len) {
            return Send(buf , len , MSG_DONTWAIT);
        }
        void Close() {
            if(_sockfd > 0) {
                close(_sockfd);
                _sockfd = -1;
            }
        }
        bool CreateServer(uint16_t port , const std::string& ip = "0.0.0.0" , bool block_flag = false) {
            if(!Create()) return false;
            if(block_flag) SetNonBlock();
            SetReuseAddress();
            if(!Bind(ip , port)) return false;
            if(!Listen()) return false;
            return true;
        }
        bool CreateClient(uint16_t port , const std::string ip) {
            if(!Create()) return false;
            // 客户端不需要显示的bind
            if(!Connect(ip , port)) return false;
            return true;
        }
        bool SetNonBlock() {
            int flag = fcntl(_sockfd , F_GETFL);
            if(flag < 0) {
                ERROR_LOG("set none block failed");
                return false;
            }
            fcntl(_sockfd , F_SETFL , flag | O_NONBLOCK);
            return true;
        }
        void SetReuseAddress() {
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
    private:
        int _sockfd;
};


class Poller;
class EventLoop;
// 对一个文件描述符的监听事件与就绪事件管理的类
using event_callback = std::function<void()>;
class Channel{
    public:
        Channel(EventLoop* loop , int fd) : _loop(loop) , _fd(fd) , _events(0) , _revents(0) {}

        // get/set方法
        int Fd() { return _fd; }
        int Events() { return _events; }
        int Revents() { return _revents; }
        void SetRevents(uint32_t revents) { _revents = revents; }
        EventLoop* GetLoop() { return _loop; }
        
        // 设置回调方法
        void SetReadCallback(const event_callback& cb) { _read_callback = cb; }
        void SetWriteCallback(const event_callback& cb) { _write_callback = cb; }
        void SetErrorCallback(const event_callback& cb) { _error_callback = cb; }
        void SetCLoseCallback(const event_callback& cb) { _close_callback = cb; }
        void SetEventCallback(const event_callback& cb) { _event_callback = cb; }
        
        // 查询是否监控了可读与可写事件
        bool IsMonitorReadable() { return _events & EPOLLIN; }
        bool IsMoinitorWrite() { return _events & EPOLLOUT; }

        // 启动或关闭事件监控
        void EnableRead() { _events |= EPOLLIN; Update(); }   
        void EnableWrite() { _events |= EPOLLOUT; Update(); }
        // 没有&=发生bug思考一下！！！
        /*客户端关闭连接，服务端epoll读事件就绪，recv读取到0，然后delete channel，但是没有释放epoll*/
        void DisableRead() { _events &= ~EPOLLIN; Update(); }   
        void DisableWrite() { _events &= ~EPOLLOUT; Update(); }
        void DisableALl() { _events = 0; Update(); }

        // 更新或移除(通过epoll模块对事件进行内核级的更新和移除)
        void Remove();
        void Update();

        // 事件处理函数，根据触发的事件调用相应的回调
        void HandlerEvent() {
            // EPOLLRDHUP：检测对方关闭 EPOLLHUP：检测连接彻底挂断
            if(_revents & EPOLLIN || _revents & EPOLLRDHUP || _revents & EPOLLPRI) {
                if(_read_callback) _read_callback(); // 当读写事件都就绪的时候，可能会close两次出现段错误
            }
            // 有可能释放连接的操作，一次只处理一个
            if(_revents & EPOLLOUT) {
                // if(_event_callback) _event_callback();
                if(_write_callback) _write_callback();
            }
            else if(_revents & EPOLLERR) {
                if(_error_callback) _error_callback();
            } else if(_revents & EPOLLHUP) {
                if(_close_callback) _close_callback();
            }
            // 通用事件的调用（还是根据业务场景决定）
            if(_event_callback) _event_callback(); // 事件处理完，刷新定时器
        }
    private:
        EventLoop* _loop;
        int _fd;
        uint32_t _events;    // 当前监听的事件
        uint32_t _revents;   // 当前触发的事件
        event_callback _read_callback;      // 可读事件回调
        event_callback _write_callback;     // 可写事件回调
        event_callback _error_callback;     // 错误事件回调
        event_callback _close_callback;     // 关闭连接事件回调
        event_callback _event_callback;     // 任意事件回调
};

// 对epoll的封装
#define MAX_EPOLLEVENTS 1024
class Poller{
    // 内部方法，执行实际的 epoll_ctl 操作
    void Update(int op , Channel* channel) {
        struct epoll_event ep;
        ep.data.fd = channel->Fd();
        ep.events = channel->Events();
        int res = epoll_ctl(_epfd , op , channel->Fd() , op == EPOLL_CTL_DEL ? nullptr : &ep);
        if(res < 0) {
            ERROR_LOG("epoll ctl failed");
            return;
        }
    }
    bool IsExistChannel(Channel* channel) {
        std::unordered_map<int , Channel*>::iterator it = _channels.find(channel->Fd());
        return it != _channels.end(); // 存在返回true，不存在返回false
    }
    public:
        Poller() {
            _epfd = epoll_create(MAX_EPOLLEVENTS);
            if(_epfd < 0) {
                ERROR_LOG("epoll create failed");
                abort(); // 退出程序
            }
        }
        ~Poller() {
            if(_epfd > 0) close(_epfd);
        }
        // 添加或修改事件监控
        void UpdateEvent(Channel* channel) {
            if(!IsExistChannel(channel)) {
                // 添加
                Update(EPOLL_CTL_ADD , channel);
                // 添加哈希表映射
                _channels[channel->Fd()] = channel;
            } else {
                // 修改
                Update(EPOLL_CTL_MOD , channel);
            }
        }
        // 移除事件监控
        void RemoveEvent(Channel* channel) {
            if(!IsExistChannel(channel)) return;
            Update(EPOLL_CTL_DEL , channel);
            _channels.erase(channel->Fd());
        }
        // 开启事件监控，返回就绪的Channel列表
        void Poll(std::vector<Channel*> *active) {
            int readyfds = epoll_wait(_epfd , _evs , MAX_EPOLLEVENTS , -1);  // 阻塞等待
            if(readyfds < 0) {
                ERROR_LOG("epoll wait failed: %s" , strerror(errno));
                abort();
            }
            for(int i = 0; i < readyfds; i++) {
                auto it = _channels.find(_evs[i].data.fd);
                assert(it != _channels.end());  // 一定存在，否则管理逻辑出现问题
                it->second->SetRevents(_evs[i].events); // 设置实际就绪的事件
                active->push_back(it->second);
            }
        }
    private:
        int _epfd;  // epoll的句柄
        struct epoll_event _evs[MAX_EPOLLEVENTS];  // 就绪事件的数组
        std::unordered_map<int , Channel*> _channels; // 描述符与Channel的映射
};

// 定时任务类
using TaskFun = std::function<void()>;
using ReleaseFun = std::function<void()>;
class TimerTask{
    public:
        TimerTask(uint64_t id , uint32_t delay , const TaskFun& task) 
            :_id(id) 
            ,_timeout(delay)
            ,_canceled(false)
            ,_task(task) 
        {}

        ~TimerTask() {
            // 析构的时候执行任务
            if(!_canceled) _task();
            // 执行清理任务
            _release_task();
        }
        uint32_t DelayTime() { return _timeout; }
        void SetReleaseFun(const ReleaseFun& release_fun) { _release_task = release_fun;}
        void SetCanceled() { _canceled = true; }
    private:
        uint64_t _id;       // 任务id
        uint32_t _timeout;  // 超时时间
        bool _canceled;       // 定时任务是否执行 默认false表示执行，true为不执行定时任务
        TaskFun _task;      // 执行的任务
        ReleaseFun _release_task; // 任务执行完毕后，将其从时间轮的哈希表中移除
};

// 时间轮
class TimerWheel{
    void RemoveHashTimerTask(uint64_t id) {
        auto it = _timers.find(id);
        if(it == _timers.end()) return;
        _timers.erase(it);
    }
    static int CreateTimerfd() {
        // CLOCK_MONOTONIC：单调时钟（从系统启动开始计时，不受系统时间修改影响）
        int timerfd = timerfd_create(CLOCK_MONOTONIC , 0);
        if(timerfd < 0) {
            ERROR_LOG("create timerfd failed");
            abort();
        }
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
    void ReadTimerfd() {
        uint64_t timer = 0;
        ssize_t res = read(_timerfd , &timer , sizeof(timer));
        if(res < 0) {
            ERROR_LOG("read timerfd failed");
            abort();
        }
    }
    // 一秒走一次
    void TickNext() {
        _tick = (_tick + 1) % _capacity;
        _timewheel[_tick].clear();    // 清空指定位置的数组，就会把数组中保存的所有管理定时器对象的shared_ptr释放掉
    }
    // 时间到了，读取timerfd，并执行当前秒应该执行的定时任务
    void OnTime() {
        // DEBUG_LOG("OnTime 执行了");
        ReadTimerfd();
        TickNext();
    }
    // 向时间轮中添加定时任务
    // 1. id: 定时任务的id 2. delay: 定时任务的延迟时间 3. task: 定时任务需要执行的方法
    void AddTimeTaskInLoop(uint64_t id , uint32_t delay , const TaskFun& task) { // 1. id: 定时任务的id 2. delay: 定时任务的延迟时间 3. task: 定时任务需要执行的方法
        DEBUG_LOG("addTimeTaskLoop: id: %ld delay: %d" , id,delay);
        // 1. 构造一个定时任务对象
        std::shared_ptr<TimerTask> time_task = std::make_shared<TimerTask>(id , delay , task);
        // 2. 将定时任务对象添加至时间轮中
        uint32_t pos = (_tick + delay) % _capacity;
        _timewheel[pos].push_back(time_task);
        // 3. 将该定时任务弱引用对象添加到哈希表中
        _timers[id] = time_task;
        // 4. 为定时任务对象注册一个回调函数，以便在完成后将其从时间轮哈希表中移除
        time_task->SetReleaseFun([this , id](){ // std::bind(&TimerWheel::RemoveHashTimerTask, this, id)
            auto it = _timers.find(id);
            if(it == _timers.end()) return;
            _timers.erase(id);
        });
    }
    // 刷新定时任务
    void RefreshTimeTaskInLoop(uint64_t id) {
        DEBUG_LOG("RefreshTimeTaskInLoop: id: %ld" , id);
        // 1. 判断该定时任务是否存在
        std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator it = _timers.find(id);
        if(it == _timers.end()) return;
        // 2. 根据哈希表中的弱引用定时对象构造新的强引用定时对象，添加到时间轮中
        std::shared_ptr<TimerTask> new_time_task = it->second.lock(); // lock() weak_ptr -> shared_ptr
        uint32_t pos = (_tick + new_time_task->DelayTime()) % _capacity;
        _timewheel[pos].push_back(new_time_task);
    }
    // 删除定时任务
    void delTimeTaskInLoop(uint64_t id) {
        // 1. 判断该定时任务是否存在
        std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator it = _timers.find(id);
        if(it == _timers.end()) return;
        // 2. 设置取消定时任务
        std::shared_ptr<TimerTask> time_task = it->second.lock();    // weak_ptr 不能直接访问成员和成员函数
        if(time_task) time_task->SetCanceled();
    }
    public:
        TimerWheel(EventLoop* loop) 
            :_capacity(60)
            ,_tick(0)
            ,_timewheel(_capacity)
            ,_loop(loop)
            ,_timerfd(CreateTimerfd())
            ,_timerfd_channel(new Channel(loop , _timerfd))
        {   
            // 开启监听定时任务的文件描述符，操作系统会每隔一秒向timerfd发送数据，导致timerfd变为可读，从而执行定时任务
            _timerfd_channel->SetReadCallback(std::bind(&TimerWheel::OnTime , this));
            _timerfd_channel->EnableRead();
        }
        // 向时间轮中添加定时任务
        void AddTimeTask(uint64_t id , uint32_t delay , const TaskFun& task);
        // 刷新定时任务
        void RefreshTimeTask(uint64_t id);
        // 删除定时任务
        void delTimeTask(uint64_t id);
        // 这个接口是有线程安全问题的，只能在当前EventLoop线程模块中调用
        bool IsExitsTimeTask(uint64_t id) {
            std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator it = _timers.find(id);
            if(it == _timers.end()) return false;
            return true;
        }
    private:
        uint32_t _capacity; // 秒级时间轮最大的延迟秒数
        uint32_t _tick;     // 时间轮指针
        std::vector<std::vector<std::shared_ptr<TimerTask>>> _timewheel; // 秒级时间轮
        std::unordered_map<uint64_t , std::weak_ptr<TimerTask>> _timers;  // [任务id ， 定时任务类的管理指针] 快速搜索已存在的定时任务对象
        EventLoop* _loop;
        // linux 定时器
        int _timerfd;
        std::unique_ptr<Channel> _timerfd_channel;
};


// 一个线程对应一个EventLoop，EventLoop也就是Reactor
// 管理自己所拥有的文件描述符的操作以及链接的管理（对Channel的一些操作和对链接的关闭等）
using Functor = std::function<void()>;
class EventLoop{
    private:
        //  执行任务队列中所有的任务
        void RunAllTasks() {
            std::vector<Functor> tasks;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _tasks.swap(tasks);
            }
            for(auto& task : tasks) {
                task();
            }
        }
        static int CreateEventfd() {
            int efd = eventfd(0 , EFD_CLOEXEC | EFD_NONBLOCK);
            if(efd < 0) {
                ERROR_LOG("create eventfd failed");
                abort();
            }
            return efd;
        }
        void ReadEventfd() {
            uint64_t res;
            ssize_t n = read(_event_fd , &res , sizeof(res));
            if(n < 0) {
                // EAGAIN表示当前没有数据可读，EINTR表示被信号打断
                if(errno == EAGAIN || errno == EINTR) { // 这些错误可以被原谅
                    return;
                }
                ERROR_LOG("read eventfd failed");
                abort();
            }
        }
        void WriteEventfd() {
            uint64_t val = 1;
            ssize_t n = write(_event_fd , &val , sizeof(val));
            if(n < 0) {
                if(errno == EAGAIN || errno == EINTR) {
                    return;
                }
                ERROR_LOG("write eventfd failed");
                abort();
            }
        }
    public:
        EventLoop()
            :_thread_id(std::this_thread::get_id())
            ,_event_fd(CreateEventfd())
            ,_event_channel(new Channel(this , _event_fd))
            ,_timer_wheel(this)
        {
            _event_channel->SetReadCallback(std::bind(&EventLoop::ReadEventfd , this));
            _event_channel->EnableRead();
        }
        void Start() {
            // 1. 监听所管理的文件描述符
            std::vector<Channel*> active;
            _poller.Poll(&active);
            // 2. 处理就绪的Channel
            for(auto active_channel : active) {
                active_channel->HandlerEvent();
            }
            // 3. 执行任务队列中的任务
            RunAllTasks();
        }
        bool IsInLoop(){ return _thread_id == std::this_thread::get_id();} // 是否在当前线程中
        // 判断将要执行的任务是否在当前线程中，若是则直接执行，不是则添加到该线程的任务队列中
        void RunInLoop(const Functor& task){
            if(IsInLoop()) {
                // DEBUG_LOG("Run In Loop 直接执行任务");
                task();
                return;
            }
            PushInLoop(task);
        }
        // 添加一个任务到该线程的任务队列中
        void PushInLoop(const Functor& task) {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _tasks.push_back(task);
            }
            // 有任务添加到任务队列中，通知该EventLoop线程去处理任务，防止EventLoop在调用Poller.Poll阻塞
            // 使用eventfd事件通知机制去进行通知
            // eventfd驱动poller模块的Poll，因此该EventLoop线程不会阻塞
            WriteEventfd();
        }
        // 对 Poller 的操作
        void UpdateEvent(Channel* channel) { _poller.UpdateEvent(channel); }
        void RemoveEvent(Channel* channel) { _poller.RemoveEvent(channel); }
        // 对 TimerWheel 的操作
        void AddTimeTask(uint64_t id , uint32_t delay , const TaskFun& task) {
            DEBUG_LOG("EventLoop::AddTimeTask");
            _timer_wheel.AddTimeTask(id , delay , task);
        }
        void RefreshTimeTask(uint64_t id) {
            _timer_wheel.RefreshTimeTask(id);
        }
        void delTimeTask(uint64_t id) {
           _timer_wheel.delTimeTask(id);
        }
        bool IsExistTimeTask(uint64_t id) {
            return _timer_wheel.IsExitsTimeTask(id);
        }
    private:
        std::thread::id _thread_id; // 当前的线程id
        int _event_fd;  // 用于其他线程通知当前线程的事件通知文件描述符
        std::unique_ptr<Channel> _event_channel;    // 管理 eventfd 的 Channel
        Poller _poller; // Poller模块
        std::vector<Functor> _tasks;    // 任务队列
        std::mutex _mutex;  // 管理任务队列的锁
        TimerWheel _timer_wheel; // 时间轮模块
};

// 更新或移除(通过epoll模块对事件进行内核级的更新和移除)
void Channel::Remove() {
    _loop->RemoveEvent(this);
}
void Channel::Update() {
    _loop->UpdateEvent(this);
}

// 向时间轮中添加定时任务
void TimerWheel::AddTimeTask(uint64_t id , uint32_t delay , const TaskFun& task) { 
    // 判断当前执行定时任务是否在该EventLoop线程中，防止线程安全问题
    // 如果业务处理放在独立线程池，处理完业务后可能需要刷新连接的活跃时间，业务线程会调用 conn->EnableInactiveRelease()，而 conn 是属于某个 EventLoop 的   
    _loop->RunInLoop(std::bind(&TimerWheel::AddTimeTaskInLoop , this , id , delay , task));
}
// 刷新定时任务
void TimerWheel::RefreshTimeTask(uint64_t id) {
    _loop->RunInLoop(std::bind(&TimerWheel::RefreshTimeTaskInLoop , this , id));
}
// 删除定时任务
void TimerWheel::delTimeTask(uint64_t id) {
    _loop->RunInLoop(std::bind(&TimerWheel::delTimeTaskInLoop , this , id));
}