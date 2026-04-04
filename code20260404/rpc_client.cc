#include <brpc/channel.h>
#include "cal.pb.h"

int main(int argc, char *argv[])
{
    // 0. 实例化ChannelOptions进行参数配置
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    // 1. 实例化Channel信道对象
    brpc::Channel chennel;
    chennel.Init("172.16.168.129:9000", &options);
    // 3. 实例化CalService_stub对象--用于发起rpc请求。
    cal::CalculateService_Stub stub(&chennel);
    brpc::Controller* cntl = new brpc::Controller;
    cal::CalculateRequest* request = new cal::CalculateRequest;
    cal::CalculateResponse* response = new cal::CalculateResponse;
    request->set_x(1024);
    request->set_y(2048);
    stub.calculate(cntl, request, response, nullptr);
    if (cntl->Failed() == true) {
        std::cout << "rpc请求失败: " << cntl->ErrorText() << std::endl;
        return -1;
    }
    std::cout << response->result() << std::endl;
    delete cntl;
    delete request;
    delete response;
    getchar();

    return 0;
}