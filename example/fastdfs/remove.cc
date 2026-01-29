#include <fastcommon/logger.h>
#include <fastdfs/fdfs_client.h>
#include <iostream>

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout<<"使用方法："<<argv[0]<<" <file_id> "<<std::endl;
        return -1;
    }

    //1.初始化日志模块
    g_log_context.log_level =LOG_ERR;
    log_init();

    //2.初始化客户端全局配置（从配置文件初始化/从缓冲区初始化）

    int ret = fdfs_client_init("./client.conf");
    if(ret != 0){
        std::cout<<"初始化全局配置失败，错误码："<<STRERROR(ret)<<std::endl;
        return -1;
    }
    //3.获取tracker服务器连接句柄
    auto tracker_server =tracker_get_connection();
    if(tracker_server == nullptr){
        std::cout<<"获取tracker服务器连接句柄失败"<<std::endl;
        return -1;
    }

    //4.进行文件的上传下载
    /**
* download file from storage server
* params:
* pTrackerServer: tracker server连接句柄
* pStorageServer: storage server连接句柄，可以为NULL
* file_id: ⽂件上传时返回的ID
* local_filename: 要另存为的⽂件路径名
* file_size: 输出参数，⽤于返回⽂件⼤⼩（必须获取）
* return: 0 success, !=0 fail, return the error code
**/

    ret= storage_delete_file1(tracker_server, nullptr, argv[1]);
    if(ret != 0){
        std::cout<<"文件删除失败，错误码："<<STRERROR(ret)<<std::endl;       
        return -1;
    }
    //5.释放资源：关闭tracker服务器连接句柄，释放客户端全局配置，释放日志模块
    tracker_close_connection(tracker_server); 
    fdfs_client_destroy();
    return 0;
}

