#include "etcd.h"
#include "log.h"
#include <thread>
#include "util.h"
#include <sstream>

namespace linsvc{

void wait_for_connect(etcd::Client &client){
    while(!client.head().get().is_ok()){
        WRN("等待与etcd服务器建立连接...");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

SvcProvider::SvcProvider(const std::string &center_addr,const std::string &svc_name, const std::string &svc_addr)
    :_center_addr(center_addr),_svc_name(svc_name),_svc_addr(svc_addr),_instance_id(linutil::Random::code()){}
    

std::string SvcProvider::make_key(){
    std::stringstream ss;
    ss<<"/"<<_svc_name<<"/"<<_instance_id;
    return ss.str();
}


bool SvcProvider::registry(){
    //实例化etcd客户端对象
    etcd::Client client(_center_addr);
    wait_for_connect(client);//等待连接成功
    //创建租约
    auto lease_resp=client.leasegrant(3).get();
    if(!lease_resp.is_ok()){
        std::cout<<"创建租约失败:"<<lease_resp.error_message()<<std::endl;  
        return false;
    }
    auto lease_id = lease_resp.value().lease();
    //重组key
    auto key=make_key();
    //根据租约向服务器添加数据
    auto resp=client.put(key,_svc_addr,lease_id).get();
    if(!resp.is_ok()){
        std::cout<<"注册服务失败:"<<resp.error_message()<<std::endl;
        return false;
    }
    //实例化保活对象，对租约进行保活
    auto handler=[this](const std::exception_ptr & e){
        this->registry();
    };

    // Create KeepAlive: first arg center_addr, second arg handler, third arg lease_id
    _keepalive.reset(new etcd::KeepAlive(_center_addr, handler, 3, lease_id));
    return true;
}    

SvcWatcher::SvcWatcher(const std::string &center_addr,SvcWatcher::ModCallback &&online,SvcWatcher::ModCallback&& offline)
    :_center_addr(center_addr),_online(std::move(online)),_offline(std::move(offline)){}

void SvcWatcher::callback(const etcd::Response &resp){
    if(resp.is_ok()==false){
        std::cout<<"监控失败:"<<resp.error_message()<<std::endl;
        return;
    }
    const auto &events=resp.events();
    for(const auto &ev:events){
        if(ev.event_type()==etcd::Event::EventType::PUT){
            std::string svc_name=parse_key(ev.kv().key());
            std::string svc_addr=ev.kv().as_string();
            DBG(" {} 服务上线节点: {}", svc_name, svc_addr);
            if(_online){
                _online(svc_name,svc_addr);
            }
        }else if(ev.event_type()==etcd::Event::EventType::DELETE_){
            std::string svc_name=parse_key(ev.prev_kv().key());
            std::string svc_addr=ev.prev_kv().as_string();
            DBG(" {} 服务下线节点: {}", svc_name, svc_addr);
            if(_offline){
                _offline(svc_name,svc_addr);
            }
        }else{
            std::cout<<"未知事件"<<std::endl;
        }

    }

}
std::string SvcWatcher::parse_key(const std::string &key){
    //  /usr/instance_id
    std::vector<std::string> arry;
    linutil::STR::split(key,"/",arry);
    return arry[0];
} 



bool SvcWatcher::watch(){
    //实例化对象，等待连接
    etcd::Client client(_center_addr);
    wait_for_connect(client);

    //浏览根目录
    auto resp=client.ls("/").get();
    if(resp.is_ok()){
        auto values=resp.values();
        for(const auto &val:values){
            std::string svc_name=parse_key(val.key());
            std::string svc_addr=val.as_string();
            if(_online){
                _online(svc_name,svc_addr);
            }
        }
    }

    auto cb=std::bind(&SvcWatcher::callback,this,std::placeholders::_1);

    _watcher.reset(new etcd::Watcher(_center_addr,"/",cb,true)); //true表示递归监控
    _watcher->Wait([this](bool cond){
        if(cond)return;
        this->watch();
    });
    return true;
}

}