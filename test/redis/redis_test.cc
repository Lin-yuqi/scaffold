#include "../../source/redis.h"

int main(){
    try{
        linredis::redis_settings settings={
            .host="192.168.86.129",
            .pwd="123456",
            .connection_pool_size=3
        };
        auto redis = linredis::RedisFactory::create_redis(settings);
        {
            auto tx=redis->transaction(false,false);
            auto r=tx.redis();

            bool ret=r.set("name","zhangsan");
            if(!ret){
                //添加失败
                std::cout<<"set name failed!"<<std::endl;
                return -1;
            }

            //     2.获取一个键值对
            auto val=r.get("name");
            if(!val){
                std::cout<<"get name failed!"<<std::endl;
                return -1;
            }
            std::cout<<"name:"<<*val<<std::endl;
            
            //     3.删除一个键值对
            r.flushall();
        }
    }catch(const sw::redis::Error &e){
        std::cout<<"Redis error: "<<e.what()<<std::endl;
    }

    return 0;
}