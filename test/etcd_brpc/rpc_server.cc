#include "../../source/rpc.h"
#include "../../source/log.h"
#include "../../source/etcd.h"
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
            });
            thr.detach();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); //模拟计算延时
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
    // 0.初始化日志对象
    linlog::linlog_init();

    const std::string etcd_addr="http://192.168.86.129:2379";
    std::string svc_name="cal";
    std::string svc_addr="192.168.86.129:9000";

    // 1.实例化计算服务对象
    CalServiceImpl* cal_service = new CalServiceImpl;
    // 1.5. 定义服务器配置对象
    auto server = linrpc::RpcServerFactory::create(9000, cal_service);

    linsvc::SvcProvider provider(etcd_addr,svc_name,svc_addr);
    provider.registry();

    server->RunUntilAskedToQuit();

    return 0;
}