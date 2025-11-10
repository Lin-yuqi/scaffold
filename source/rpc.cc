#include "rpc.h"
#include "log.h"

namespace linrpc{
    
    void Channels::Insert(const std::string &addr){
        std::lock_guard<std::mutex> lock(_mtx);
        brpc::ChannelOptions options;
        options.protocol = "baidu_std";
        options.timeout_ms = 30000;
        auto channel = std::make_shared<brpc::Channel>();
        channel->Init(addr.c_str(), &options);

        _channels.emplace_back(addr,channel);
    }

    void Channels::Remove(const std::string& addr){
        std::lock_guard<std::mutex> lock(_mtx);
        for(auto it = _channels.begin(); it != _channels.end(); ++it){
            if(it->first == addr){
                _channels.erase(it);
                break;
            }
        }
    }

    ChannelPtr Channels::GetChannel(){
        std::lock_guard<std::mutex> lock(_mtx);
        if(_channels.size() == 0){
            return ChannelPtr();
        }
        return _channels[(_idx++) % _channels.size()].second;
    }

    std::optional<std::string> Channels::GetAddr(){
        std::lock_guard<std::mutex> lock(_mtx);
        if(_channels.size() == 0){
            return std::nullopt;
        }
        return _channels[(_idx++) % _channels.size()].first;
    }

    Channels::ptr SvcChannel::_Channels(const std::string &svc_name){
        std::lock_guard<std::mutex> lock(_mtx);
        auto it = _maps.find(svc_name);
        if(it != _maps.end()){
            return it->second;
        }
        return nullptr;
    }

    void SvcChannel::SetWatch(const std::string& svc_name){
        std::lock_guard<std::mutex> lock(_mtx);
        auto channels = std::make_shared<Channels>();
        _maps.insert({svc_name,channels});
    }

    void SvcChannel::AddChannel(const std::string& svc_name,const std::string& addr){
        auto channels = _Channels(svc_name);
        if(channels){
            channels->Insert(addr);
        }else{
            std::cout<<"服务 "<<svc_name<<" 不存在，无法添加节点 "<<addr<<std::endl;
        }
    }

    void SvcChannel::DelChannel(const std::string& svc_name,const std::string& addr){
        auto channels = _Channels(svc_name);
        if(channels){
            channels->Remove(addr);
        }else{
            std::cout<<"服务 "<<svc_name<<" 不存在，无法删除节点 "<<addr<<std::endl;    
        }
    }

    ChannelPtr SvcChannel::GetChannel(const std::string& svc_name){
        auto channels = _Channels(svc_name);
        if(channels){
            return channels->GetChannel();
        }else{
            std::cout<<"服务 "<<svc_name<<" 不存在，无法获取节点信道"<<std::endl;
            return ChannelPtr();
        }
    }

    std::optional<std::string> SvcChannel::GetAddr(const std::string& svc_name){
        auto channels = _Channels(svc_name);
        if(channels){
            return channels->GetAddr();
        }else{
            std::cout<<"服务 "<<svc_name<<" 不存在，无法获取节点地址"<<std::endl;
            return std::nullopt;
        }
    }

    google::protobuf::Closure* ClosureFactory::create(callback_t &&cb){
        auto obj = std::make_shared<Object>();
        obj->callback = std::move(cb);
        return brpc::NewCallback(&ClosureFactory::asyncCallback, obj);
    } 
    
    void ClosureFactory::asyncCallback(const Object::ptr obj) {
        obj->callback();
    }

    std::shared_ptr<brpc::Server> RpcServerFactory::create(int port, google::protobuf::Service *svc){
        brpc::ServerOptions options;
        options.idle_timeout_sec= -1; //不超时

        auto server = std::make_shared<brpc::Server>();

        int ret = server->AddService(svc, brpc::SERVER_OWNS_SERVICE);
        if(ret != 0) {
            std::cout<<"Fail to add service"<<std::endl;
            return nullptr;
        }
        if (server->Start(port, &options) != 0) {
            std::cout<<"Fail to start RPC server"<<std::endl;
            return nullptr;
        }
        return server;
    }

}
