#pragma once
#include <etcd/Client.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/KeepAlive.hpp>

namespace linsvc{

    extern void wait_for_connect(etcd::Client &client);

    class SvcProvider{
        public:
            using ptr=std::shared_ptr<SvcProvider>;
            SvcProvider(const std::string &center_addr,const std::string &svc_name, const std::string &svc_addr);
            // `register` is a reserved keyword in C++; use a different name
            bool registry();
        private:
            std::string make_key();
        private:
            std::string _center_addr;   //注册中心地址
            std::string _svc_name;  //服务名称
            std::string _svc_addr;  //服务节点地址
            std::string _instance_id;   //节点标识
            std::shared_ptr<etcd::KeepAlive> _keepalive; //租约续约对象
    };

    class SvcWatcher{
        public:
            using ptr=std::shared_ptr<SvcWatcher>;
            //回调函数，在产生事件时，传入两个参数 1服务名称 2服务地址
            using ModCallback =std::function<void(std::string,std::string)>;
            SvcWatcher(const std::string &center_addr,ModCallback &&online,ModCallback&& offline);
            // 直接监控根目录
            bool watch();
        private:
            void callback(const etcd::Response &resp);
            std::string parse_key(const std::string &key);
        private:
            std::string _center_addr;   
            ModCallback _online;
            ModCallback _offline;
            std::shared_ptr<etcd::Watcher> _watcher;
    };
}