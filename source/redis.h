#pragma once
#include <chrono>
#include <sw/redis++/redis.h>
#include <sw/redis++/queued_redis.h>
#include <iostream>

namespace linredis{
    struct redis_settings{
        std::string host;
        std::string usr="default";
        std::string pwd;
        int port=6379;
        int db=0;
        size_t connection_pool_size=5;
    };
    class RedisFactory{
        public:
        static std::shared_ptr<sw::redis::Redis> create_redis(const redis_settings& settings);
    };
}
