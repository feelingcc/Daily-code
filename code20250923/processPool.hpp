#pragma once

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <cstdlib>
#include <sys/wait.h>
#include "tasks.hpp"

// 这个类用来描述主进程与子进程间的任务通信通道
class Channel {
    public:
        Channel(int wfd , pid_t process_id) :_wfd(wfd) , _process_id(process_id)  {
            _name = "child process: pid - " + std::to_string(_process_id) + " wfd - " + std::to_string(_wfd);
        }
        int get_wfd() { return _wfd; }
        pid_t get_process_id () { return _process_id; }
        std::string& get_name() { return _name; }
        // 向 wfd 管道发送 taskcode 任务码
        void send(int taskcode) {
            ssize_t n = write(_wfd , &taskcode , sizeof(taskcode));
            (void)n;
        }
    private:
        int _wfd;           // 管道写入端文件描述符，主进程通过此fd向子进程发送数据
        pid_t _process_id;  // 目标子进程的进程ID，用于唯一标识任务接收方
        std::string _name;  // 子进程的逻辑名称，用于日志记录和进程管理
};

// 这个类用来管理所有的信道
class ChannelManger {
    public:
        // 插入一个信道
        void insert (int wfd , pid_t cid) {
            _channels.emplace_back(wfd , cid);  // 直接构造，无需构造 + 拷贝/构造 + 移动
        }

        // 销毁所有的信道
        void destroy_all () {
            for (size_t i = 0; i < _channels.size(); i++) {
                close(_channels[i].get_wfd());
                // std::cout << "close child process name: " << _channels[i].get_name() << std::endl;
            }
        }

        // 轮询方式选择一个子进程信道
        Channel& select() {
            auto& c = _channels[_next];
            _next++;
            _next %= _channels.size();
            return c;
        }

        // 回收所有的子进程
        void recycle_all () {
            for (size_t i = 0; i < _channels.size(); i++)  {
                int rid = waitpid(_channels[i].get_process_id() , nullptr , 0);
                if (rid < 0) {
                    std::cerr << "waitpid error" << std::endl;
                    return;
                }
                std::cout << "wait success - name: " << _channels[i].get_name() << std::endl;
            }
        }

        void end() {
            for (size_t i = 0; i < _channels.size(); i++) {
                close(_channels[i].get_wfd());
                std::cout << "close child process name: " << _channels[i].get_name() << std::endl;
                waitpid(_channels[i].get_process_id() , nullptr , 0);
                std::cout << "wait success - name: " << _channels[i].get_name() << std::endl;
            }
        }
    private:
        std::vector<Channel> _channels;
        int _next = 0; 
};

// 这个类用来描述一个进程池
class ProcessPool {
    public:
        ProcessPool(int process_count = 3) :_process_count(process_count) {
            // 注册任务
            _tasks.add(print_log);
            _tasks.add(download);
            _tasks.add(upload);
        }

        ~ProcessPool() {}

        void work (int rfd) {
            while (true) {
                int taskcode = 0;   // 存储从管道读取的任务指令码
                ssize_t n = read(rfd , &taskcode , sizeof(taskcode));
                if (n > 0) {
                    // 成功读取到数据，但需要检查是否读取了完整的数据
                    if (n != sizeof(taskcode)) {
                        continue;   // 跳过本次不完整数据，继续下一次读取
                    }
                    // 完整读取到任务指令码，此处应添加任务处理逻辑
                    std::cout << "child process pid: " << getpid() << " 收到任务码: " << taskcode << " ";
                    _tasks.execute(taskcode);   // 任务处理
                    // sleep(1);
                } else if (n == 0) {
                    // 读取到文件结束符(EOF)，表示主进程已关闭管道写入端
                    // 这通常意味着主进程要求该子进程终止，子进程将被SIGPIPE(13)信号杀死
                    std::cout << "pid: " << getpid() << " child process exit" << std::endl;
                    break;
                } else {
                    std::cerr << "read error" << std::endl;
                    break;
                }
            }
        }

        bool create () {
            for (int i = 0; i < _process_count; i++) {
                // 1. 创建匿名管道
                int fds[2] = {0};
                int n = pipe(fds);
                if (n < 0) {
                    std::cerr << "pipe create error" << std::endl;
                    return false;
                }
                
                // 2. 创建子进程
                pid_t id = fork();
                if (id < 0) {
                    std::cerr << "child process create error" << std::endl;
                    return false;
                } else if (id == 0) {
                    // 子进程
                    // 关闭子进程中从父进程继承的所有写端文件描述符
                    // 父进程所有的写端都被存储在 ChannelManger::std::vector<Channel> _channels; 
                    // 第一次循环继承父进程的 ChannelManger._cpcm 中的 _channels为空，无论父子进程执行顺序，因为当下面父进程 insert 时会触发 ChannelManger _cpcm; 写时拷贝
                    // 第二次循环继承父进程的 ChannelManger._cpcm 中的 _channels仅有一个 Channel 元素，而该 Channel 元素中存储的写端是4
                    // 第三次...
                    // 因此，只需每次遍历 ChannelManger._cpcm 中的 _channels 关闭所有的 Channel 里的 _wfd 即可。
                    _cpcm.destroy_all();
                    // 创建单向通信信道，关闭不需要的文件描述符
                    close(fds[1]);
                    work(fds[0]);
                    close(fds[0]);
                    exit(0);
                }
                // 父进程
                // 创建单向通信信道，关闭不需要的文件描述符
                close(fds[0]);
                // 管理创建的子进程和通信信道
                _cpcm.insert(fds[1] , id);
            }
        
            return true;
        }

        void destroy () {
            // 3. 关闭所有的信道`
            // _cpcm.destroy_all();
            // // 4. 回收所有的子进程
            // _cpcm.recycle_all();

            _cpcm.end();
        }

        // 主进程为进程池中的子进程分配任务
        void run (int n) {
            while (n--) {
                int taskcode = _tasks.get_taskcode();
                auto& c = _cpcm.select();

                std::cout << "主进程向 - " << c.get_wfd() << " - 信道写入任务码 - " << " - " << taskcode << std::endl; 
                c.send(taskcode);
                sleep(1);
            }
        }
    private:
        Tasks _tasks;
        ChannelManger _cpcm;
        int _process_count;     // 进程池中进程的个数
};
