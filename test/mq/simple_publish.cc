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
    //实例化发布者对象
    auto Publisher = linmq::MQFactory::create<linmq::Publisher>(client, settings);
    //发布消息
    Publisher->publish("hello linmq");
    return 0;
}