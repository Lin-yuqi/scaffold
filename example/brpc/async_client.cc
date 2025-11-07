#include <brpc/channel.h>
#include "cal.pb.h"
#include <unistd.h>

void callback(brpc::Controller* cntl,cal::AddReq* request,cal::AddRsp* response){
    std::unique_ptr<brpc::Controller> cntl_guard(cntl);
    std::unique_ptr<cal::AddRsp> response_guard(response);
    std::unique_ptr<cal::AddReq> request_guard(request);
    if (cntl->Failed()) {
        //调用失败
        std::cout<<"call failed, error:"<<cntl->ErrorText()<<std::endl;
        return;
    }
    std::cout<<"result:"<<response->sum()<<std::endl;

}

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
    brpc::Controller* cntl = new brpc::Controller;
    cntl->set_timeout_ms(5000);
    //4.创建请求对象和响应对象
    cal::AddReq* request = new cal::AddReq;
    cal::AddRsp* response = new cal::AddRsp;
    request->set_a(10);
    request->set_b(20);
    google::protobuf::Closure* done =
        brpc::NewCallback(callback, cntl, request, response);
    stub.Add(cntl, request, response, done);
    std::cout<<"async call sent"<<std::endl;
    getchar(); //等待异步调用完成
    return 0;
}