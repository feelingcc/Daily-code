#include <cstdint>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unistd.h>

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
    public:
        TimerWheel() 
            :_capacity(60)
            ,_tick(0)
            ,_timewheel(_capacity)
        {}
        // 向时间轮中添加定时任务
        void AddTimeTask(uint64_t id , uint32_t delay , const TaskFun& task) { // 1. id: 定时任务的id 2. delay: 定时任务的延迟时间 3. task: 定时任务需要执行的方法
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
        void RefreshTimeTask(uint64_t id) {
            // 1. 判断该定时任务是否存在
            std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator it = _timers.find(id);
            if(it == _timers.end()) return;
            // 2. 根据哈希表中的弱引用定时对象构造新的强引用定时对象，添加到时间轮中
            std::shared_ptr<TimerTask> new_time_task = it->second.lock(); // lock() weak_ptr -> shared_ptr
            uint32_t pos = (_tick + new_time_task->DelayTime()) % _capacity;
            _timewheel[pos].push_back(new_time_task);
        }
        // 删除定时任务
        void delTimeTask(uint64_t id) {
            // 1. 判断该定时任务是否存在
            std::unordered_map<uint64_t , std::weak_ptr<TimerTask>>::iterator it = _timers.find(id);
            if(it == _timers.end()) return;
            // 2. 设置取消定时任务
            std::shared_ptr<TimerTask> time_task = it->second.lock();    // weak_ptr 不能直接访问成员和成员函数
            if(time_task) time_task->SetCanceled();
        }
        // 一秒走一次
        void TickNext() {
            _tick = (_tick + 1) % _capacity;
            _timewheel[_tick].clear();    // 清空指定位置的数组，就会把数组中保存的所有管理定时器对象的shared_ptr释放掉
        }
    private:
        uint32_t _capacity; // 秒级时间轮最大的延迟秒数
        uint32_t _tick;     // 时间轮指针
        std::vector<std::vector<std::shared_ptr<TimerTask>>> _timewheel; // 秒级时间轮
        std::unordered_map<uint64_t , std::weak_ptr<TimerTask>> _timers;  // [任务id ， 定时任务类的管理指针] 快速搜索已存在的定时任务对象
};

class Test {
    public:
        Test() {std::cout << "构造" << std::endl;}
        ~Test() {std::cout << "析构" << std::endl;}
};

void DelTest(Test *t) {
    delete t;
}

int main()
{
    TimerWheel tw;

    Test *t = new Test();

    tw.AddTimeTask(888, 5, std::bind(DelTest, t));

    for(int i = 0; i < 5; i++) {
        sleep(1);
        tw.RefreshTimeTask(888);//刷新定时任务
        tw.TickNext();//向后移动秒针
        std::cout << "刷新了一下定时任务，重新需要5s中后才会销毁\n";
    }
    tw.delTimeTask(888);
    while(1) {
        sleep(1);
        std::cout << "-------------------\n";
        tw.TickNext();//向后移动秒针
    }
    return 0;
}
