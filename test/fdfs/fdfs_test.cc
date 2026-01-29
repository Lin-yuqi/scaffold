/*
    1.从文件上传
    2.下载到文件
        1.将文件上传到fastdfs服务器
        2.从fastdfs服务器下载文件到本地
        3通过md5sum校验上传下载的文件是否一致
    3.从内存上传
    4.下载到内存
    5.多线程并行测试，多线程同时下载 
    6.删除
        删除两个文件，重新进行下载
    
*/

#include "../../source/log.h"
#include "../../source/util.h"
#include "../../source/fdfs.h"//一定要最后被包含
std::string file_test(){
    // 1.将文件上传到fastdfs服务器
    auto id = linfdfs::FDFSClient::upload_from_file("./makefile");
    if(!id){
        ERR("文件上传失败");
        abort();
    }
    // 2.从fastdfs服务器下载文件到本地
    bool ret= linfdfs::FDFSClient::download_to_file(*id, "./makefile.down1");
    if(!ret){
        ERR("文件下载失败");
        abort();
    }
    // 3通过md5sum校验上传下载的文件是否一致
    DBG("下载文件成功！file_id:{}", *id);
    return *id;
      
}

std::string buff_test(){
    std::string body;
    linutil::FUTIL::read("./makefile", body);

    auto id = linfdfs::FDFSClient::upload_from_buff(body);
    if(!id){
        ERR("文件上传失败");
        abort();
    }
    std::string buff;
    bool ret= linfdfs::FDFSClient::download_to_buff(*id, buff);
    if(!ret){
        ERR("文件下载失败");
        abort();
    }
    linutil::FUTIL::write("./makefile.down2", buff);
    DBG("下载文件成功！file_id:{}", *id);
    return *id;
}

void mt_test(std::vector<std::string> &ids, std::mutex &mtx){
    std::vector<std::thread> threads;
    for(int i=0;i<3;i++){
        threads.emplace_back([&](){
            for(int i=0;i<3;i++){
                auto id = linfdfs::FDFSClient::upload_from_file("./makefile");
                if(!id){
                    ERR("文件上传失败");
                    abort();
                }
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    ids.push_back(*id);
                }
                DBG("上传文件成功！file_id:{}", *id);
            }
        });
    }
    for(auto &t:threads){
        t.join();
    }
}

void remove_test(std::vector<std::string> &ids){
    //删除两个文件，重新进行下载
    for(auto &id:ids){
        bool ret= linfdfs::FDFSClient::remove(id);
        if(!ret){
            ERR("文件删除失败");
            abort();
        }
        DBG("文件{}删除成功", id);
    }
}


int main(){
    linlog::linlog_init();

    linfdfs::fdfs_settings settings={
        .trackers={"192.168.86.129:22122"}
    };

    linfdfs::FDFSClient::init(settings);

    std::mutex mtx;

    // auto id1=file_test();
    // auto id2=buff_test();
    std::vector<std::string> arr;
    // remove_test(arr);

    arr.clear();

    mt_test(arr,mtx);
    
    remove_test(arr);

    linfdfs::FDFSClient::destroy();
    return 0;
}
