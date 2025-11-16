#include "../../source/util.h"
#include "../../source/log.h"
#include "../../source/etcd.h"

int main(){
    linlog::linlog_init();
    //实力服务注册对象
    linsvc::SvcProvider provider("http://192.168.86.129:2379","usr","192.168.86.129:9000");
    //注册
    provider.registry();
    getchar();
    return 0;
}