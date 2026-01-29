#include <brpc/channel.h>
#include "cal.pb.h"

int main(){
    //  0.实例化channel对象    
    brpc::Channel channel;
    // 1.定义channel配置对象
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    //2.初始化channel对象
    if (channel.Init("127.0.0.1:9000", &options) != 0) {
        //初始化失败
        printf("channel init failed\n");
        return -1;
    }
    //3.创建stub对象
    cal::Calservice_Stub stub(&channel);
    brpc::Controller cntl;
    //4.创建请求对象和响应对象
    cal::AddReq request;
    cal::AddRsp response;
    request.set_a(10);
    request.set_b(20);
    stub.Add(&cntl, &request, &response, nullptr);
    if (cntl.Failed()) {
        //调用失败
        std::cout<<"call failed, error:"<<cntl.ErrorText()<<std::endl;
        return -1;
    }


    std::cout<<"result:"<<response.sum()<<std::endl;

    return 0;
}