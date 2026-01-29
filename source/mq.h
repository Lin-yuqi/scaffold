#pragma once
#include <amqpcpp.h>
#include <ev.h>
#include <amqpcpp/libev.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>

namespace linmq{
    const std::string DIRECT = "direct";
    const std::string FANOUT = "fanout";
    const std::string TOPIC = "topic";
    const std::string HEADERS = "headers";
    const std::string DELAYED = "delayed";
    const std::string DLX_PREFIX = "dlx_";
    const std::string DEAD_LETTER_EXCHANGE = "x-dead-letter-exchange";
    const std::string DEAD_LETTER_ROUTING_KEY = "x-dead-letter-routing-key";
    const std::string MESSAGE_TTL = "x-message-ttl";

    struct declare_settings{
        std::string exchange;
        std::string queue;
        std::string routing_key;
        std::string exchange_type;
        size_t ttl = 0;

        std::string dlx_exchange() const;
        std::string dlx_routing_key() const;
        std::string dlx_queue() const;
    };
    extern AMQP::ExchangeType get_exchange_type(const std::string& type);
    using MessageCallback = std::function<void(const char*, size_t)>;

    class MQClient{
        public:
            using ptr = std::shared_ptr<MQClient>;
            MQClient(const std::string& addr);//构造成员，启动循环
            ~MQClient();//发送异步请求，结束事件循环，等到线程退出

            //声明交换机和队列，绑定交换机和队列，如果是延时队列，还要创建配套的死信交换机和死信队列
            //声明绑定成功后，需要等待，等待实际交换机和队列声明成功，再返回
            void declare(const declare_settings& settings);
            bool publish(const std::string& exchange, const std::string& routing_key, const std::string& body);
            void consume(const std::string& queue, const MessageCallback& cb);
                         

            void wait();

        private:
            static void async_cb(struct ev_loop* loop, struct ev_async* w, int revents);
        
            void _declare_normal(const declare_settings& settings,AMQP::Table & args,bool is_dlx=false);
        private:
            std::mutex _mtx;
            std::condition_variable _cv;
            struct ev_loop* _ev_loop;
            struct ev_async _ev_async;
            AMQP::LibEvHandler _handler;
            AMQP::TcpConnection _connection;
            AMQP::TcpChannel _channel;
            std::thread _async_thread;  
    };

    class Publisher{
        public:
            using ptr =  std::shared_ptr<Publisher>;
            //对成员进行初始化，并声明套件内的交换机和队列
            Publisher(const MQClient::ptr &client, const declare_settings& settings);

            bool publish(const std::string& body);
        private:
        MQClient::ptr _client;
        declare_settings _settings;

    };


    class Subscriber{
        public:
            using ptr =  std::shared_ptr<Subscriber>;
            //对成员进行初始化，并声明套件内的交换机和队列
            Subscriber(const MQClient::ptr &client, const declare_settings& settings);
            //订阅时，如果是延时队列，则实际订阅的是配套的死信队列
            void consume(const MessageCallback&& cb);
        private:
            MQClient::ptr _client;
            declare_settings _settings;
            MessageCallback _cb;
    };

    class MQFactory{
        public:
            template<typename R, typename... Args>
            static std::shared_ptr<R> create(Args&&... args){
                return std::make_shared<R>(std::forward<Args>(args)...);
            }
    };
}
