#include <fastcommon/logger.h>
#include <fastdfs/fdfs_client.h>
#include <iostream>


int main(){
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

    /*
    上传⽂件：
    pTrackerServer: tracker server连接句柄
    pStorageServer: storage server连接句柄，可以为NULL
    store_path_index: 获取storage节点时顺便获取的索引, 若没有主动获取节点则可以
    为0
    local_filename: 本地要上传的⽂件路径名
    file_ext_name: ⽂件扩展名，可以为null
    meta_list: ⽂件元信息数组
    meta_count: 元信息元素数量，若没有传递元信息数组则设置为0
    group_name: 若没有主动获取storage节点，则可以设置为null
    file_id: 输出参数--是⽂件上传保存成功后分配的标识ID
    return: 0 success, !=0 fail, return the error code
    */

    //4.进行文件的上传下载
    char file_id[256+1];
    ret=storage_upload_by_filename1(tracker_server, nullptr, 0,"./makefile",nullptr,nullptr,0,nullptr,file_id);
    if(ret != 0){
        std::cout<<"文件上传失败，错误码："<<STRERROR(ret)<<std::endl;
        return -1;
    }
    std::cout<<"文件上传成功，文件ID为："<<file_id<<std::endl;
    //5.释放资源：关闭tracker服务器连接句柄，释放客户端全局配置，释放日志模块
    tracker_close_connection(tracker_server); 
    fdfs_client_destroy();
    return 0;
}



