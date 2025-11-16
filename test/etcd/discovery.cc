#include "../../source/util.h"
#include "../../source/log.h"
#include "../../source/etcd.h"

void online(const std::string &svc_name,const std::string &svc_addr){
    INF("服务上线: {} {}", svc_name.c_str(), svc_addr.c_str());
}
void offline(const std::string &svc_name,const std::string &svc_addr){
    INF("服务下线: {} {}", svc_name.c_str(), svc_addr.c_str());
}

int main(){

    //先初始化日志对象
    linlog::linlog_init();

    //实力服务发现对象
    linsvc::SvcWatcher watcher("http://192.168.86.129:2379",online,offline);
    //注册
    watcher.watch();
    getchar();
    
    return 0;
}