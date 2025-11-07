#include "cal.pb.h"
#include <brpc/server.h>
#include <butil/logging.h>
#include <thread>

class CalServiceImpl : public cal::Calservice {
    public:
        CalServiceImpl(){}
        ~CalServiceImpl() override{}

        virtual void Add(::google::protobuf::RpcController* controller,
            const ::cal::AddReq* request,
            ::cal::AddRsp* response,
            ::google::protobuf::Closure* done) override {
            // Use ClosureGuard to ensure `done->Run()` is called exactly once
            // The guard will call done->Run() in its destructor, so do NOT
            // call done->Run() manually here to avoid double-run / double-free.

            // done->Run() will be invoked by done_guard when this function returns
            std::thread thr([=](){
                brpc::ClosureGuard done_guard(done);
                int result=request->a()+request->b();
                response->set_sum(result);
                std::this_thread::sleep_for(std::chrono::seconds(2));
            });
            thr.detach();
            std::cout<<"async processing in server thread"<<std::endl;
        }
};

int main(){
    //1.实例化计算服务对象
    CalServiceImpl cal_service;
    //1.5. 定义服务器配置对象
    brpc::ServerOptions options;
    options.idle_timeout_sec= -1; //不超时 
    //2.实例化服务器对象
    brpc::Server server;
    //2.5添加服务
    int ret = server.AddService(&cal_service, brpc::SERVER_DOESNT_OWN_SERVICE);
    //3.启动服务器
    if (server.Start(9000, &options) != 0) {
        LOG(ERROR) << "Fail to start server";
        return -1;
    }
    //4.等待服务器结束
    server.RunUntilAskedToQuit();


}