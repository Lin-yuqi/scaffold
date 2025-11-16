/*
    日志封装：
        1.防止头文件重复包含
        2.包含头文件
        3.声明命名空间
        4.声明全局日志器
        5.声明日志配置结构体
        6.声明全局日志初始化接口
        7.封装日志输出宏
*/

//防止头文件重复包含
#pragma once
//包含头文件
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>

//声明命名空间
namespace linlog {

//声明日志配置结构体
struct log_settings{
    bool async = false;               //是否异步
    int level = 1;                    //日志级别 1-debug;2-info;3-warn;4-error; 6-off
    std::string path = "stdout";      //日志文件路径
    std::string format = "[%H:%M:%S][%-7l]:%v";               //日志格式 [%H:%M:%S][%-7l]:%v
};

//声明全局日志初始化接口
extern void linlog_init(const log_settings& settings=log_settings());

//声明全局日志器
extern std::shared_ptr<spdlog::logger> g_logger;

#define FMT_PREFIX std::string("[{}:{}]: ")
#define DBG(fmt,...) linlog::g_logger->debug(FMT_PREFIX+ fmt,__FILE__,__LINE__,##__VA_ARGS__)
#define INF(fmt,...) linlog::g_logger->info(FMT_PREFIX+ fmt,__FILE__,__LINE__,##__VA_ARGS__)
#define WRN(fmt,...) linlog::g_logger->warn(FMT_PREFIX+ fmt,__FILE__,__LINE__,##__VA_ARGS__)
#define ERR(fmt,...) linlog::g_logger->error(FMT_PREFIX+ fmt,__FILE__,__LINE__,##__VA_ARGS__)

}
