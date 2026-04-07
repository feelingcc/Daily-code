#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <iostream>
#include <string>
#include <thread>

void get(const std::string& host ,  const std::string& key , std::shared_ptr<etcd::Watcher>& watcher) {
    // 1.创建与etcd交互的客户端
    etcd::Client client(host);
    // 2.浏览所监控目录下的所有数据
    etcd::Response resp = client.ls(key).get();
    while(!resp.is_ok()) {
        std::cout << "目前还没有能提供该服务的节点: " << resp.error_message() << std::endl; 
        std::this_thread::sleep_for(std::chrono::seconds(1));
        resp = client.ls(key).get();
    }
    // 3.获取结果
    std::vector<etcd::Value> values = resp.values();
    for(auto& value : values) {
        std::cout << "key: " << value.key() << " " << "value: " << value.as_string() << std::endl;
    }
    // 4.数据监控
    auto watcher_callback = [](const etcd::Response& resp){
        if (!resp.is_ok()) {
            std::cout << "监控出错: " << resp.error_message() << std::endl;
            return;
        }   
        // resp.events() 返回一个事件数组，因为一次 watcher 触发可能包含多个变化
        const std::vector<etcd::Event>& events = resp.events();
        for(auto& event : events) {
            if(event.event_type() == etcd::Event::EventType::PUT) {
                std::cout << event.kv().key() << "数据改变: " << event.prev_kv().as_string() << "->" << event.kv().as_string() << std::endl;
            } else if(event.event_type() == etcd::Event::EventType::DELETE_) {
                std::cout << event.prev_kv().key() << "数据删除!" << std::endl;
            } else {
                assert(false);
            }
        }
    };
    // watcher_callback回调触发时机：监控的目录下，只要有key新增或修改某个key的值或某个key被删除了就会调用该回调处理
    watcher.reset(new etcd::Watcher(host, key , watcher_callback , true));  //true表示递归监控目录下所有数据
    // 5.开启异步非阻塞监控
    watcher->Wait([host , key , &watcher](bool cancel) mutable {
        // lambda 默认捕获参数为 const 熟悉，可以添加 mutable 关键字
        if(cancel) {
            // 任务被主动取消
            return;
        }
        get(host , key , watcher);
    });
}

int main() {

    // 调用服务方，进行服务发现
    std::string etcd_host = "http://172.16.168.129:2379";
    std::string key = "/";
    // 创建监控服务对象
    std::shared_ptr<etcd::Watcher> watcher;
    get(etcd_host , key , watcher);
    getchar();

    return 0;
}