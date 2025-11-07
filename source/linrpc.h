#pragma once
#include <butil/logging.h>
#include <brpc/channel.h>
#include <brpc/server.h>
#include <mutex>
#include <optional>


namespace linrpc{
    using ChannelPtr = std::shared_ptr<brpc::Channel>;
    class Channels{
    public:
        using ptr=std::shared_ptr<Channels>;
    public:
        // 增加节点
        void Insert(const std::string &addr);
        // 删除节点
        void Remove(const std::string& addr);
        // 获取节点信道
        ChannelPtr GetChannel();
        // 获取节点信息
        std::optional<std::string> GetAddr();

    private:
        std::mutex _mtx;
        std::vector<std::pair<std::string, ChannelPtr>> _channels;
        uint32_t _idx = 0;
        std::string _svc_name;
    };

    class SvcChannel{
    public:
        using ptr=std::shared_ptr<SvcChannel>;
        

        //加入服务
        void SetWatch(const std::string& svc_name);
        //为某个节点添加信道
        void AddChannel(const std::string& svc_name,const std::string& addr);
        //给某个节点删除信道
        void DelChannel(const std::string& svc_name,const std::string& addr);
        //获取某个节点的信道
        ChannelPtr GetChannel(const std::string& svc_name);
        //获取某个节点的信息
        std::optional<std::string> GetAddr(const std::string& svc_name);
    private:
        Channels::ptr _Channels(const std::string &svc_name);
        std::mutex _mtx;
        std::map<std::string,Channels::ptr> _maps;
    };


class ClosureFactory {
    public:
        using callback_t = std::function<void()>;
        static google::protobuf::Closure* create(callback_t &&cb);
    private:
        struct Object{
            using ptr = std::shared_ptr<Object>;
            callback_t callback;
        };
        static void asyncCallback(const Object::ptr obj);
};

class RpcServerFactory {
    public:
        // 默认svc是堆上new出来的对象，将管理权移交给rpc服务器进行管理
        static std::shared_ptr<brpc::Server> create(int port, google::protobuf::Service *svc);
};

}