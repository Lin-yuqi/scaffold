#include "linlog.h"

namespace linlog {
    //声明全局日志器
    std::shared_ptr<spdlog::logger> g_logger;
    //声明全局日志初始化接口
    void linlog_init(const log_settings& settings){
        //判断日志器类型
        //判断输出目标
        //创建日志器
        if(settings.async){
            if(settings.path == "stdout"){
                //异步控制台日志器
                g_logger = spdlog::stdout_color_mt<spdlog::async_factory>("stdout_logger");
            }else{
                g_logger = spdlog::basic_logger_mt<spdlog::async_factory>("file_logger",settings.path);
            }
        }else{
            if(settings.path == "stdout"){
                //同步控制台日志器
                g_logger = spdlog::stdout_color_mt("stdout_logger");
            }else{
                g_logger = spdlog::basic_logger_mt("file_logger",settings.path);
            }
        }
        //设置等级
        g_logger->set_level(spdlog::level::level_enum(settings.level));
        //设置格式
        g_logger->set_pattern(settings.format);

    }
}