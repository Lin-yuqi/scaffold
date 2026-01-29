#include "../../source/linrpc.h"
#include "../../source/linlog.h"
#include "cal.pb.h"

class CalServiceImpl : public cal::CalService {
    public:
        CalServiceImpl(){}
        ~CalServiceImpl() override{}

        void Add(::google::protobuf::RpcController* controller,
            const ::cal::AddReq* request,
            ::cal::AddRsp* response,
            ::google::protobuf::Closure* done) override {
            // Use ClosureGuard to ensure `done->Run()` is called exactly once
            // The guard will call done->Run() in its destructor, so do NOT
            // call done->Run() manually here to avoid double-run / double-free.

            // done->Run() will be invoked by done_guard when this function returns
            std::thread thr([=](){
                brpc::ClosureGuard done_guard(done);
                int result=request->num1()+request->num2();
                response->set_result(result);
                std::this_thread::sleep_for(std::chrono::seconds(2));
            });
            thr.detach();
            std::cout<<"async processing in server thread"<<std::endl;
        }

        void Hello(::google::protobuf::RpcController* controller,
            const ::cal::helloReq* request,
            ::cal::helloRsp* response,
            ::google::protobuf::Closure* done) override {
            //别忘了设置Closure管理
            brpc::ClosureGuard done_guard(done); 
            brpc::Controller* cntl = (brpc::Controller*)controller;
            const auto& headers = cntl->http_request();
            std::cout << "Method:" << brpc::HttpMethod2Str(headers.method()) << std::endl;
            std::cout << "Body:" << cntl->request_attachment().to_string() << std::endl;

            cntl->response_attachment().append("回显:" + cntl->request_attachment().to_string());
            cntl->http_response().set_status_code(200);
        }
};

int main(){
    // 1.实例化计算服务对象
    CalServiceImpl* cal_service = new CalServiceImpl;
    // 1.5. 定义服务器配置对象
    auto server = linrpc::RpcServerFactory::create(9000, cal_service);

    server->RunUntilAskedToQuit();

    return 0;
}