#pragma once

#include <functional>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

void print_log () { std::cout << "打印日志" << std::endl; }
void download () { std::cout << "下载任务" << std::endl; }
void upload () { std::cout << "上传任务" << std::endl; }

class Tasks {
    public:
        Tasks() {
            srand(time(nullptr));
        }

        void add(std::function<void()> task) {
            tasks.push_back(task);
        }

        // 随机分配一个任务
        int get_taskcode () {
            if (tasks.empty()) {
                return -1;
            }
            return rand() % tasks.size();   
        }

        // 执行任务
        bool execute (int taskcode) {
            if (taskcode < 0 || taskcode >= tasks.size()) {
                return false;
            }
            tasks[taskcode]();
            return true;
        }
    private:
        std::vector<std::function<void()>> tasks;
};