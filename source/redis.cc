#include "redis.h"

namespace linredis{
    std::shared_ptr<sw::redis::Redis> RedisFactory::create_redis(const redis_settings& settings){
        sw::redis::ConnectionOptions connection_options{
            .host = settings.host,
            .port = settings.port,
            .user = settings.usr,
            .password = settings.pwd,
            .db = settings.db
        };
        sw::redis::ConnectionPoolOptions pool_options{
            .size = settings.connection_pool_size
        };
        return std::make_shared<sw::redis::Redis>(connection_options, pool_options);
    }
}