#include <curl/curl.h>
#include <iostream>
#include <sstream>

size_t callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    std::stringstream* ss = (std::stringstream*)userdata;
    ss->read(ptr, size * nmemb);
    return ss->gcount();
}

int main(){
    const std::string usrname="lin-yq@foxmail.com";
    const std::string paswd="ysyltjhgqfbfebed";
    // const std::string paswd="lxyretwdsbrjdijb";
    const std::string url="smtps://smtp.qq.com:465";
    const std::string from="lin-yq@foxmail.com";
    const std::string to="8202241112@csu.edu.cn";
    
    //初始化全局配置

    auto ret =curl_global_init(CURL_GLOBAL_DEFAULT);
    if(ret != CURLE_OK){
        std::cerr << "curl_global_init failed: " << curl_easy_strerror(ret) << std::endl;
        return -1;
    }
    //构造操作句柄
    auto curl = curl_easy_init();
    if(!curl){
        std::cerr << "curl_easy_init failed" << std::endl;
        curl_global_cleanup();
        return -1;
    }
    //设置请求参数： url paswd body
    ret=curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt URL failed: " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    } 
    ret=curl_easy_setopt(curl, CURLOPT_USERNAME, usrname.c_str());//设置用户名
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt USERNAME failed: " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    ret=curl_easy_setopt(curl, CURLOPT_PASSWORD, paswd.c_str());    //设置密码
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt PASSWORD failed: " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=LOGIN");

    ret=curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());    //设置发件人
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt MAIL_FROM failed: " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    struct curl_slist* cs = nullptr;
    cs = curl_slist_append(cs, to.c_str());
    ret=curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, cs); //设置收件人
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt MAIL_RCPT failed: " << curl_easy_strerror(ret) << std::endl;
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }


    std::stringstream ss;
    // ss<<"From:"<<from<<"\r\n";
    // ss<<"To:"<<to<<"\r\n";//入果有多个收件人，则收件人之间以英文逗号进行间隔
    ss<<"Subject: 内容\r\n";//邮件主题
    ss<<"Content-Type: text/html; charset=UTF-8\r\n";
    ss<<"\r\n";
    ss<<"<html><head><title>验证码</title></head><body><p>验证码：<b>123456</b></p><p>验证码将在3分钟后失效</p></body></html>\r\n";//邮件内容
    
    
    
    
    ret=curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&ss);
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt READDATA failed: " << curl_easy_strerror(ret) << std::endl;
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    ret=curl_easy_setopt(curl, CURLOPT_READFUNCTION, callback);
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt READFUNCTION failed: " << curl_easy_strerror(ret) << std::endl;
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    ret=curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L); //设置为上传模式
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_setopt UPLOAD failed: " << curl_easy_strerror(ret) << std::endl;
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }

//     // 强制使用 TLS 1.2 或更高版本
// curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
// // 启用详细的协议调试信息（非常重要）
// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    //执行请求
    ret=curl_easy_perform(curl);
    if(ret != CURLE_OK){
        std::cerr << "curl_easy_perform failed: " << curl_easy_strerror(ret) << std::endl;
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return -1;
    }
    //清理资源
    curl_slist_free_all(cs);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}