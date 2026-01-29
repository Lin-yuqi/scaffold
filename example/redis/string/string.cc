/*
    1.实例化redis客户端对象
    2.通过客户端对象操作string类型数据
        1.添加一个键值对
        2.获取一个键值对
        3.删除一个键值对
        4.对键值对重点数据进行自增&自减操作
*/

#include <sw/redis++/redis.h>
#include <iostream>
#include <chrono>
int main(){
    try{
        //1.实例化redis客户端对象
        sw::redis::ConnectionOptions conn_opts={
            .host="192.168.86.129",
            .password="123456"  
        };

        sw::redis::ConnectionPoolOptions pool_opts={
            .size=10,
            .connection_idle_time=std::chrono::milliseconds(3600)
        };
        sw::redis::Redis redis(conn_opts,pool_opts);
        

        //2.通过客户端对象操作string类型数据
        //    1.添加一个键值对
        bool ret=redis.set("name","zhangsan");
        if(!ret){
            //添加失败
            std::cout<<"set name failed!"<<std::endl;
            return -1;
        }

        //     2.获取一个键值对
        auto val=redis.get("name");
        if(!val){
            std::cout<<"get name failed!"<<std::endl;
            return -1;
        }
        std::cout<<"name:"<<*val<<std::endl;
        
        //     3.删除一个键值对
        redis.del("name");

        //     4.对键值对重点数据进行自增&自减操作
        redis.set("age","18");
        redis.incrby("age",1); //age=19
        std::cout<<"age:"<<*redis.get("age")<<std::endl;
        redis.decrby("age",2); //age=17
        std::cout<<"age:"<<*redis.get("age")<<std::endl;
    }catch(const sw::redis::Error &e){
        std::cout<<"redis exception:"<<e.what()<<std::endl;
        return -1;
    }

    return 0;
}