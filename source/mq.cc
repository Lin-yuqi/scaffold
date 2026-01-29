#include "mq.h"
#include "log.h"

namespace linmq{

    AMQP::ExchangeType get_exchange_type(const std::string& type){
        if(type == DIRECT){
            return AMQP::ExchangeType::direct;
        }else if(type == FANOUT){
            return AMQP::ExchangeType::fanout;
        }else if(type == TOPIC){
            return AMQP::ExchangeType::topic;
        }else if(type == HEADERS){
            return AMQP::ExchangeType::headers;
        }else if(type == DELAYED){
            return AMQP::ExchangeType::direct;
        }
        ERR("交换机类型错误：{}", type);
        abort();
    }
    
    std::string declare_settings::dlx_exchange()const{
        return DLX_PREFIX + exchange;
    }
    std::string declare_settings::dlx_routing_key()const{
        return DLX_PREFIX + routing_key;
    }
    std::string declare_settings::dlx_queue()const{
        return DLX_PREFIX + queue;
    }

    MQClient::MQClient(const std::string& addr)
        :_ev_loop(EV_DEFAULT)
        ,_handler(_ev_loop)
        ,_connection(&_handler, AMQP::Address(addr))
        ,_channel(&_connection) 
        ,_async_thread(std::thread([this](){ev_run(_ev_loop);})){
    }

    MQClient::~MQClient(){
        ev_async_init(&_ev_async, MQClient::async_cb);
        ev_async_start(_ev_loop, &_ev_async);
        ev_async_send(_ev_loop, &_ev_async);
        _async_thread.join();

    }

    void MQClient::async_cb(struct ev_loop* loop, struct ev_async* w, int revents){
        ev_break(loop, EVBREAK_ALL); //不能跨线程调用
    }


    void MQClient::_declare_normal(const declare_settings& settings, AMQP::Table & args,bool is_dlx){
        std::unique_lock<std::mutex> lock(_mtx);
        // 根据参数is_dlx判断是否是死信队列，取出要使用的交换机、队列和路由键名称
        std::string exchange,queue,routing_key;
        if(is_dlx){
            exchange = settings.dlx_exchange();
            queue = settings.dlx_queue();
            routing_key = settings.dlx_routing_key();
        }else{
            exchange = settings.exchange;
            queue = settings.queue;
            routing_key = settings.routing_key;
        }
        AMQP::ExchangeType ex_type = get_exchange_type(settings.exchange_type);
        // 声明交换机
        _channel.declareExchange(exchange, ex_type)
            .onSuccess([=,this](){
                // 声明队列
                _channel.declareQueue(queue,args)
                    .onSuccess([=,this](const std::string &name, uint32_t messagecount, uint32_t consumercount){
                        _channel.bindQueue(exchange,queue,routing_key)
                            .onSuccess([=,this](){
                                //声明成功，通知外部
                                std::unique_lock<std::mutex> lock(_mtx);
                                _cv.notify_all();
                            })
                            .onError([=,this](const char* message){
                                ERR("队列{}绑定到交换机{}失败，错误信息:{}", queue, exchange, message);
                                abort();
                            });
                    })
                    .onError([=,this](const char* message){
                        ERR("队列声明失败，错误信息:{}", message);
                        abort();
                    });
                    
            })
            .onError([=,this](const char* message){
                ERR("交换机声明失败，错误信息:{}", message);
                abort();
            });
        _cv.wait(lock);
    }

    void MQClient::declare(const declare_settings& settings){
        //判断是否是延时队列
        AMQP::Table args;
        if(settings.exchange_type==DELAYED){
            _declare_normal(settings,args, true); //声明死信交换机和死信队列
            args[DEAD_LETTER_EXCHANGE]=settings.dlx_exchange();
            args[DEAD_LETTER_ROUTING_KEY]=settings.dlx_routing_key();
            args[MESSAGE_TTL]=settings.ttl; // 消息过期时间5秒
        }   
        _declare_normal(settings, args,false); //声明正常交换机和正常队列

    }
    bool MQClient::publish(const std::string& exchange, const std::string& routing_key, const std::string& body){
        return _channel.publish(exchange, routing_key, body);
    }   
    void MQClient::consume(const std::string& queue, const MessageCallback& cb){
        _channel.consume(queue)
            .onMessage([=,this](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered){
                cb(message.body(), message.bodySize());
                _channel.ack(deliveryTag);
            })
            .onError([&](const char* message){
                ERR("订阅队列失败，错误信息:{}", message);
                abort();
            })
            .onSuccess([&] (){
                DBG("订阅队列{}成功", queue);
            });    
    }

    void MQClient::wait(){
        _async_thread.join();
    }


    Publisher::Publisher(const MQClient::ptr& client, const declare_settings& settings)
        :_client(client), _settings(settings){
        _client->declare(_settings);
    }

    bool Publisher::publish(const std::string& body){
        //发布消息不管是普通的还是延时的，统一发到普通队列
        return _client->publish(_settings.exchange, _settings.routing_key, body);
    }


    Subscriber::Subscriber(const MQClient::ptr& client, const declare_settings& settings)
        :_client(client), _settings(settings){
        _client->declare(_settings);
    }
    void Subscriber::consume(const MessageCallback&& cb){
        _cb = std::move(cb);
        //如果是延时队列，订阅死信队列
        if(_settings.exchange_type == DELAYED){
            _client->consume(_settings.dlx_queue(), _cb);
        }else{
            _client->consume(_settings.queue, _cb);
        }
    }
} //namespace linmq