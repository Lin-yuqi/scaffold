#include "../../source/rpc.h"
#include "../../source/log.h"
#include "../../source/etcd.h"
#include "cal.pb.h"

int main(){
    // 初始化日志（必须在使用 DBG/INF 等宏之前调用）
    linlog::linlog_init();

    const std::string etcd_addr="http://192.168.86.129:2379";
    const std::string svc_name="cal";


    // 0.实例化服务节点管理对象
    linrpc::SvcChannel svcc;
    // 1.添加服务节点
    svcc.SetWatch(svc_name);

    auto online_cb=std::bind(&linrpc::SvcChannel::AddChannel,&svcc,std::placeholders::_1,std::placeholders::_2);
    auto offline_cb=std::bind(&linrpc::SvcChannel::DelChannel,&svcc,std::placeholders::_1,std::placeholders::_2);   

    linsvc::SvcWatcher watcher(etcd_addr,online_cb,offline_cb);
    watcher.watch();


    // 2.获取服务节点信道
    auto channel = svcc.GetChannel(svc_name);
    while(!channel){
        ERR("获取服务信道失败，等待中...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        channel=svcc.GetChannel(svc_name);
    }
    // 3.发起异步RPC调用
    brpc::Controller* cntl = new brpc::Controller;
    cntl->set_timeout_ms(5000);
    cal::AddReq* request = new cal::AddReq;
    cal::AddRsp* response = new cal::AddRsp;
    request->set_num1(1314);
    request->set_num2(520);

    auto done = linrpc::ClosureFactory::create([=](){
        std::unique_ptr<brpc::Controller> cntl_guard(cntl);
        std::unique_ptr<cal::AddRsp> response_guard(response);
        std::unique_ptr<cal::AddReq> request_guard(request);
        if (cntl->Failed()) {
            //调用失败
            std::cout<<"call failed, error:"<<cntl->ErrorText()<<std::endl;
            return;
        }
        std::cout<<"result:"<<response->result()<<std::endl;
    });
    cal::CalService_Stub stub(channel.get());
    stub.Add(cntl, request, response, done);
    std::cout<<"async call sent"<<std::endl;
    getchar(); //等待异步调用完成
    return 0;
}


