#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <iostream>
#include <string>
#include <memory>

void put(
    const std::string &host,
    const std::string &key,
    const std::string &val,
    std::shared_ptr<etcd::KeepAlive> &keepalive,
    int ttl = 3
) {
    // 1.创建与etcd交互的客户端
    etcd::Client client(host);
    // 2.创建并获取存活ttl时间的租约
    etcd::Response lease_resp = client.leasegrant(ttl).get();
    if(!lease_resp.is_ok()) {
        std::cout << "创建租约失败: " << lease_resp.error_message() << std::endl;
        return;
    }
    // 3.获取租约id
    int64_t lease_id = lease_resp.value().lease();
    // 4.注册key，val，并与租约绑定
    etcd::Response resp = client.put(key , val , lease_id).get();
    if(!resp.is_ok()) {
        std::cout << "数据新增失败: " << resp.error_message() << std::endl;
        return;
    }
    // 5.创建保活对象
    auto keepalive_callback = [host , key , val , &keepalive](std::exception_ptr eptr){
        // 注意悬空引用，而keepalive是在主程序中的，并且还需要修改keepalive，所以必须使用引用
        put(host , key , val , keepalive);
    };
    // keepalive_callback KeepAlive 内部的后台线程因网络断开、etcd 服务端无响应、心跳发送失败等原因意外停止时触发
    keepalive.reset(new etcd::KeepAlive(host , keepalive_callback  , ttl , lease_id));
}

int main() {

    // 提供服务方，进行服务的注册
    std::string etcd_host = "http://172.16.168.129:2379";
    std::string key = "/user/instance1";
    std::string val = "172.16.168.129:9000";
    // 创建租约保活对象
    std::shared_ptr<etcd::KeepAlive> keep_alive;
    put(etcd_host , key , val , keep_alive);
    getchar();

    return 0;
}