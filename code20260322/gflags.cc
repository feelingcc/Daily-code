#include <iostream>
#include <gflags/gflags.h>

DEFINE_bool(reuse_addr , true , "是否开启地址重用");
DEFINE_string(ip , "127.0.0.1" , "默认的服务器的ip地址");
DEFINE_int32(port , 8888 , "默认的服务器端口");

int main(int argc , char* argv[]) {

    google::ParseCommandLineFlags(&argc , &argv , true);
    google::SetCommandLineOption("flagfile", "gflags.conf");
    std::cout << FLAGS_reuse_addr << std::endl;
    std::cout << FLAGS_ip << std::endl;
    std::cout << FLAGS_port << std::endl;

    return 0;
}