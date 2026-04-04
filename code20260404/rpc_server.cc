#include <butil/logging.h>
#include <brpc/server.h>
#include "cal.pb.h"
#include <iostream>

class CalculateServiceImpl : public cal::CalculateService {
    public:
    void calculate
    (
        ::google::protobuf::RpcController* controller,
        const ::cal::CalculateRequest* request,
        ::cal::CalculateResponse* response,
        ::google::protobuf::Closure* done
    ) override {
        brpc::ClosureGuard done_guard(done);
        response->set_result(request->x() + request->y());
    }
};

int main() {

    // 0.关闭brpc的日志输出
    logging::LoggingSettings logging_setting;
    logging_setting.logging_dest = logging::LOG_TO_NONE;
    logging::InitLogging(logging_setting);
    // 1.实例化计算服务对象
    CalculateServiceImpl calculate_service;
    // 2.定义服务器配置对象
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;
    // 3.实例化服务器对象
    brpc::Server server;
    // 4.向服务器添加服务
    int res = server.AddService(&calculate_service , brpc::SERVER_DOESNT_OWN_SERVICE);
    if(res == -1) {
        std::cout << "addService failed" << std::endl;
        return -1;
    }
    // 5.启动服务器
    server.Start(9000 , &options);
    // 6.等待服务器停止
    server.RunUntilAskedToQuit();

    return 0;
}