#include <etcd/Client.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/KeepAlive.hpp>

//实例化
//创建租约
//添加数据 设置租约
//保活


int main(){
    const std::string url="http://192.168.86.129:2379";

    etcd::Client cli(url);
    //创建租约
    auto lease_resp=cli.leasegrant(3).get();
    if(!lease_resp.is_ok()){
        std::cout<<"创建租约失败"<<lease_resp.error_message()<<std::endl;
    }

    
    //获取租约id
    auto lease_id=lease_resp.value().lease();


    auto resp=cli.put("nb","sb",lease_id).get();
    if(!resp.is_ok()){
        std::cout<<"put key failed"<<resp.error_message()<<std::endl;
        return -1;
    }

    auto handler=[](std::exception_ptr e){
        try {
            if(e) std::rethrow_exception(e);
        } catch (const std::exception &e) {
            std::cout<<"watcher exception:"<<e.what()<<std::endl;
        } 
    };

    // 租约保活
    etcd::KeepAlive alive(url,handler,3,lease_id);
    std::cout<<"put key success"<<std::endl;
    getchar(); //等待保活完成

    return 0;
}