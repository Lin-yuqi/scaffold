//  实例化libev网络通信模块句柄
//  实例化connection对象    
//  实例化channel对象
//  声明交换机
//  声明队列
//  绑定队列到交换机
//  发布消息到交换机
//  启动事件循环
//  销毁句柄，释放资源.

#include <amqpcpp.h>
#include <ev.h>
#include <amqpcpp/libev.h>
#include <iostream>

void declarecomponent(AMQP::TcpChannel& channel,const std::string& exchange_name,const std::string& queue_name,const std::string& routing_key){
    channel.declareExchange(exchange_name,AMQP::ExchangeType::direct,0)
        .onSuccess([&](){
            std::cout<<"交换机声明成功"<<std::endl;
            //  声明队列
            channel.declareQueue(queue_name,0)
                .onSuccess([&](const std::string &name, uint32_t messagecount, uint32_t consumercount){
                    std::cout<<"队列声明成功，队列名:"<<name<<std::endl;
                    std::cout<<"队列中消息数:"<<messagecount<<",消费者数:"<<consumercount<<std::endl;
                    //  绑定队列到交换机
                    channel.bindQueue(exchange_name,queue_name,routing_key)
                        .onSuccess([&](){
                            std::cout<<"队列绑定到交换机成功"<<std::endl;
                            channel.consume(queue_name)
                                .onMessage([&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered){
                                    std::string msg_body(message.body(), message.bodySize());
                                    std::cout<<"收到消息:"<<msg_body<<std::endl;
                                    channel.ack(deliveryTag);
                                })
                                .onError([&](const char* message){
                                    std::cout<<"订阅队列失败，错误信息:"<<message<<std::endl;
                                    abort();
                                })
                                .onSuccess([&] (){
                                    std::cout<<"订阅队列成功"<<std::endl;
                                });                         
                        })
                        .onError([&](const char* message){
                            std::cout<<"队列绑定到交换机失败，错误信息:"<<message<<std::endl;
                            abort();
                        });

                })
                .onError([&](const char* message){
                    std::cout<<"队列声明失败，错误信息:"<<message<<std::endl;
                    abort();
                });

        })
        .onError([&](const char* message){
            std::cout<<"交换机声明失败，错误信息:"<<message<<std::endl;
            abort();
        });

}

int main(){
    const std::string url="amqp://admin:123456@192.168.86.129:5672/";
    const std::string dlx_exchange="dlx_exchange";
    const std::string dlx_queue="dlx_queue";
    const std::string dlx_routing_key="dlx_routing_key";

    auto* loop = EV_DEFAULT;    //网络通信事件循环句柄
    AMQP::LibEvHandler handler(loop); //与amqpcpp库结合的句柄
//  实例化connection对象    
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
//  实例化channel对象
    AMQP::TcpChannel channel(&connection); //与rabbitmq服务器通信的信道
    declarecomponent(channel,dlx_exchange,dlx_queue,dlx_routing_key);

    // 启动事件循环
    ev_run(loop, 0);
    return 0;
}
