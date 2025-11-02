#include <iostream>
#include <gflags/gflags.h>

DEFINE_bool(reuse_addr,true,"是否重用");
DEFINE_int32(port,8080,"监听端口");
DEFINE_string(ip,"0.0.0.0","监听ip");
DEFINE_double(pi,3.1415,"圆周率");

int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << "reuse_addr: " << FLAGS_reuse_addr << std::endl;
    std::cout << "port: " << FLAGS_port << std::endl;
    std::cout << "ip: " << FLAGS_ip << std::endl;   
    std::cout << "pi: " << FLAGS_pi << std::endl;

    return 0;
}