#include <sw/redis++/redis.h>
#include <iostream>
#include <chrono>
#include <sw/redis++/transaction.h>
#include <sw/redis++/queued_redis.h>
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
        
        {
            auto transaction=redis.transaction();
            auto redis=transaction.redis();
            while(true){
                try{
                    redis.watch("age");
                    bool res=redis.exists("age");
                    if(!res){
                        break;
                    }
                    transaction.incrby("age",1);
                    transaction.exec();
                    break;
                }catch( const sw::redis::WatchError &e){
                    std::cout<<"transaction exception:"<<e.what()<<std::endl;
                    continue;
                }catch( const sw::redis::Error &e ){
                    std::cout<<"redis exception:"<<e.what()<<std::endl;
                    throw;
                }
            } 
        }
    }catch(const sw::redis::Error &e){
        std::cout<<"redis exception:"<<e.what()<<std::endl;
        return -1;
    }

    return 0;
}