#include <linlog.h>
#include <gflags/gflags.h>

//1.通过gflags定义要捕获的参数
DEFINE_bool(async, false, "是否异步日志");
DEFINE_int32(level, 1, "日志级别 1-debug;2-info;3-warn;4-error;6-off");
DEFINE_string(path, "stdout", "日志文件路径");
DEFINE_string(format, "[%H:%M:%S][%-7l] %v", "日志格式");


int main(int argc,char* argv[]){
    //解析命令行参数
    google::ParseCommandLineFlags(&argc, &argv, true);
    //初始化日志器
    linlog::log_settings settings={
        .async = FLAGS_async,
        .level = FLAGS_level,
        .path = FLAGS_path,
        .format = FLAGS_format
    };
    linlog::linlog_init(settings);
    //输出日志
    DBG("this is debug log");
    INF("小明今年{}岁了",18);
    WRN("小美的成绩是{}",95.5);
    ERR("程序出现异常，错误码是{}",-1);
    
    return 0;
}