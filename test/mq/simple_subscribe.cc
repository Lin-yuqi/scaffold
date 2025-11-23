#include "../../source/mq.h"
#include "../../source/log.h"

int main(){
    //定义代理服务器url
    linlog::linlog_init();
    const std::string url="amqp://admin:123456@192.168.86.129:5672/";
    //定义配置信息
    linmq::declare_settings settings={
        .exchange="test_exchange",
        .queue="test_queue",
        .routing_key="test_routing_key",
        .exchange_type="direct"
    };
    //实例化mQ客户端对象
    auto client = linmq::MQFactory::create<linmq::MQClient>(url);
    //实例化订阅者对象
    auto Subscriber = linmq::MQFactory::create<linmq::Subscriber>(client, settings);
    //订阅消息
    Subscriber->consume([](const char* body, size_t size){
        std::string msg(body, size);
        printf("收到消息:%s\n", msg.c_str());
    });

    client->wait();    
    return 0;
}