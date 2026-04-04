#include <brpc/channel.h>
#include <brpc/callback.h>
#include "cal.pb.h"

void callback(
    brpc::Controller* cntl,
    cal::CalculateRequest* request,
    cal::CalculateResponse* response
) {
    std::shared_ptr<brpc::Controller> cntl_guard(cntl);
    std::shared_ptr<cal::CalculateRequest> request_guard(request);
    std::shared_ptr<cal::CalculateResponse> response_guard(response);
    if (cntl_guard->Failed() == true) {
        std::cout << "rpc请求失败: " << cntl_guard->ErrorText() << std::endl;
        return;
    }
    std::cout << response_guard->result() << std::endl;
}

int main(int argc, char *argv[])
{
    // 0. 实例化ChannelOptions进行参数配置b   
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    // 1. 实例化Channel信道对象
    brpc::Channel chennel;
    chennel.Init("172.16.168.129:9000", &options);
    // 3. 实例化CalService_stub对象--用于发起rpc请求。
    cal::CalculateService_Stub stub(&chennel);
    brpc::Controller* cntl = new brpc::Controller;
    cntl->set_timeout_ms(4000);
    cal::CalculateRequest* request = new cal::CalculateRequest;
    cal::CalculateResponse* response = new cal::CalculateResponse;
    request->set_x(1024);
    request->set_y(2048);
    google::protobuf::Closure* closure = brpc::NewCallback(callback , cntl , request , response);
    stub.calculate(cntl, request, response, closure);
    std::cout << "=========" << std::endl;

    getchar();

    return 0;
}