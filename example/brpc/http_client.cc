#include <brpc/channel.h>
#include "cal.pb.h"

int main(){
    //  0.实例化channel对象    
    brpc::Channel channel;
    // 1.定义channel配置对象
    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_HTTP;
    //2.初始化channel对象
    if (channel.Init("127.0.0.1:9000", &options) != 0) {
        //初始化失败
        printf("channel init failed\n");
        return -1;
    }
    //3.创建stub对象
    //cal::Calservice_Stub stub(&channel);
    brpc::Controller cntl;
    //4.创建请求对象和响应对象
    // cal::AddReq request;
    // cal::AddRsp response;
    // request.set_a(10);
    // request.set_b(20);
    // stub.Add(&cntl, &request, &response, nullptr);
    // if (cntl.Failed()) {
    //     //调用失败
    //     std::cout<<"call failed, error:"<<cntl.ErrorText()<<std::endl;
    //     return -1;
    // }

    //http只使用cntl对象

    cntl.http_request().set_method(brpc::HTTP_METHOD_POST);//设置请求方法

    cntl.http_request().uri().set_path("/Calservice/Add");

    cntl.http_request().SetHeader("Content-Type", "application/json");
    cntl.request_attachment().append(R"({"a":10,"b":20})");

    channel.CallMethod(nullptr, &cntl, nullptr, nullptr, nullptr);     
    if(cntl.Failed()) {
        //调用失败
        std::cout<<"call failed, error:"<<cntl.ErrorText()<<std::endl;
        return -1;
    }

    std::cout<<"result:"<<cntl.response_attachment()<<std::endl;
    return 0;
}