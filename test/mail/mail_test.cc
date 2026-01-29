#include "../../source/mail.h"
#include <memory>
#include "../../source/log.h"

int main(){
    linlog::linlog_init();
    //构造邮件服务配置
    linmail::mail_settings setting={
        .username="lin-yq@foxmail.com",
        .password="ysyltjhgqfbfebed",
        .url="smtps://smtp.qq.com:465",
        .from="lin-yq@foxmail.com"
    };
    //实例化邮件客户端对象
    auto mail_client =std::make_unique<linmail::MailClient>(setting);
    //发送邮件
    bool ret = mail_client->send("8202241112@csu.edu.cn","520");
    if(!ret){
        std::cout<<"send mail failed"<<std::endl;
        return -1;
    }
    std::cout<<"send mail success"<<std::endl;
    return 0;
}