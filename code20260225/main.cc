#include "server.hpp"

int main() {

    return 0;
}

// 日志模块的设计
// int main() {

//     INFO_LOG("hello");
//     INFO_LOG("%s %d" , "hello world" , 10);

//     return 0;
// }

// void test_buffer() {
//     Buffer buffer;
//     int i = 300;
//     while(i--) {
//         std::string msg = "hello world" + std::to_string(i) + "\n";
//         buffer.writeStringAndPush(msg);
//     }
//     // std::string str = buffer.ReadAsString(buffer.ReadableSize());
//     // std::cout << str << std::endl;

//     std::string line = buffer.getLineAndPop();
//     std::cout << line << std::endl;
//     line = buffer.getLineAndPop();
//     std::cout << line << std::endl;

//     // Buffer b(buffer);
//     // std::string str = b.readAsString(b.getReadableSize());
//     // std::cout << str << std::endl;

//     while(true) {
//         std::string line = buffer.getLineAndPop();
//         if(line.empty()) break;
//         std::cout << line << std::endl;
//     }
// }

// int main() {

//     test_buffer();

//     return 0;
// }

// 测试 Any 类
// int main() {

//     Any any(10);
//     std::cout << *any.get<int>() << std::endl;

//     any = std::string("hello");
//     std::cout << *any.get<std::string>() << std::endl;

//     return 0;
// }

// 测试时间轮
// class Test {
//     public:
//         Test() {std::cout << "构造" << std::endl;}
//         ~Test() {std::cout << "析构" << std::endl;}
// };

// void DelTest(Test *t) {
//     delete t;
// }

// int main()
// {
//     TimerWheel tw;

//     Test *t = new Test();

//     tw.addTimerTask(888, 5, std::bind(DelTest, t));

//     for(int i = 0; i < 5; i++) {
//         sleep(1);
//         tw.refreshTimerTask(888);//刷新定时任务
//         tw.nextTick();//向后移动秒针
//         std::cout << "刷新了一下定时任务，重新需要5s中后才会销毁\n";
//     }
//     tw.delTimerTask(888);
//     while(1) {
//         sleep(1);
//         std::cout << "-------------------\n";
//         tw.nextTick();//向后移动秒针
//     }
//     return 0;
// }