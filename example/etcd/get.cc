#include <etcd/Client.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/KeepAlive.hpp>

//实例化
//获取数据
//输出数据
//进行数据监控 -- 编写回调处理函数

void callback(const etcd::Response &resp){
    if(!resp.is_ok()){
        std::cout<<"监控出错: "<<resp.error_message()<<std::endl;
        return;
    }
    const auto& events=resp.events();
    for(const auto& e: events){
        if(e.event_type()==etcd::Event::EventType::PUT){
            std::cout<<e.prev_kv().key()<<"数据改变"<<e.prev_kv().as_string()<<"->"<<e.kv().as_string()<<std::endl;
        } else if(e.event_type()==etcd::Event::EventType::DELETE_){
            std::cout<<e.prev_kv().key()<<"数据被删除"<<std::endl;
        }else{
            std::cout<<"无效操作"<<std::endl;
        }
    } 
}



int main(){
    const std::string url="http://192.168.86.129:2379";
    etcd::Client cli(url);

    auto resp=cli.get("nb").get();
    if(!resp.is_ok()){
        std::cout<<"get key failed"<<resp.error_message()<<std::endl;
        return -1;  
    }

    std::cout<<"key:"<<resp.value().as_string()<<std::endl;

    //数据监控
    etcd::Watcher watcher(url, "nb", callback,false);
    watcher.Wait();

    return 0;
}