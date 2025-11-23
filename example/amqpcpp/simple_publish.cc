//
//  实例化libev网络通信模块句柄
//  实例化connection对象    
//  实例化channel对象
//  声明交换机
//  声明队列
//  绑定队列到交换机
//  发布消息到交换机
//  启动事件循环
//  销毁句柄，释放资源

#include <amqpcpp.h>
#include <ev.h>
#include <amqpcpp/libev.h>
#include <iostream>

int main(){
//  定义rabbitmq容器访问的url
    const std::string url="amqp://admin:123456@192.168.86.129:5672/";
    const std::string exchange_name="my_exchange";
    const std::string queue_name="my_queue";
    const std::string routing_key="my_routing_key";
//  实例化libev网络通信模块句柄
    auto* loop = EV_DEFAULT;    //网络通信事件循环句柄
    AMQP::LibEvHandler handler(loop); //与amqpcpp库结合的句柄
//  实例化connection对象    
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
//  实例化channel对象
    AMQP::TcpChannel channel(&connection); //与rabbitmq服务器通信的信道
//  声明交换机
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
                            //  发布消息到交换机
                            std::string message="Hello, AMQP-CPP!";
                            bool ret=channel.publish(exchange_name,routing_key,message);
                            if(!ret){
                                std::cout<<"消息发布失败"<<std::endl;
                                abort();
                            }
                            std::cout<<"消息发布成功"<<std::endl;
                            
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

    // 启动事件循环
    ev_run(loop, 0);
    //  销毁句柄，释放资源
    ev_break(loop);
    return 0;

}



