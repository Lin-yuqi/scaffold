#include "mail.h"
#include "log.h"

namespace linmail{
    MailClient::MailClient(const mail_settings& settings):_settings(settings){
        auto ret = curl_global_init(CURL_GLOBAL_DEFAULT);
        if(ret != CURLE_OK){
            ERR("curl_global_init failed: {}", curl_easy_strerror(ret));
            abort();
        } 
    }
    //释放全局配置资源
    MailClient::~MailClient(){
        curl_global_cleanup();
    }
    //发送邮件验证码
    bool MailClient::send(const std::string& to, const std::string& code) {
        auto body=CodeBody(to, code);
        auto curl = curl_easy_init();
        if(!curl){
            ERR("curl_easy_init failed");
            return false;
        }
        auto ret= curl_easy_setopt(curl, CURLOPT_USERNAME, _settings.username.c_str());
        if(ret != CURLE_OK) return FAILED(ret, curl);

        ret= curl_easy_setopt(curl, CURLOPT_PASSWORD, _settings.password.c_str());
        if(ret != CURLE_OK) return FAILED(ret, curl);
        
        ret= curl_easy_setopt(curl, CURLOPT_URL, _settings.url.c_str());
        if(ret != CURLE_OK) return FAILED(ret, curl);
        
        ret= curl_easy_setopt(curl, CURLOPT_MAIL_FROM, _settings.from.c_str());
        if(ret != CURLE_OK) return FAILED(ret, curl);
        curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=LOGIN");

        struct curl_slist* cs = nullptr;    
        cs = curl_slist_append(cs, to.c_str());
        ret= curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, cs);
        if(ret != CURLE_OK){return FAILED(ret, curl);}

        ret= curl_easy_setopt(curl, CURLOPT_READDATA, (void*)&body);
        if(ret != CURLE_OK){return FAILED(ret, curl);}

        ret= curl_easy_setopt(curl, CURLOPT_READFUNCTION, MailClient::call_back);
        if(ret != CURLE_OK){return FAILED(ret, curl);}

        ret= curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        if(ret != CURLE_OK){return FAILED(ret, curl);}

        ret= curl_easy_perform(curl);
        if(ret != CURLE_OK){
            ERR("curl_easy_perform failed: {}", curl_easy_strerror(ret));
            curl_slist_free_all(cs);
            curl_easy_cleanup(curl);
            return false;   
        }
        curl_slist_free_all(cs);
        curl_easy_cleanup(curl);
        DBG("send mail to {} success", to);
        return true;
    }
    bool MailClient::FAILED(CURLcode ret, CURL* curl){
        ERR("curl_easy_setopt failed: {}", curl_easy_strerror(ret));
        curl_easy_cleanup(curl);
        return false;
    }

    std::stringstream MailClient::CodeBody(const std::string& to, const std::string& code){
        std::stringstream ss;
        ss << "To: " << to << "\r\n";
        ss << "From: " << _settings.from << "\r\n";
        ss << "Subject: " << _subject << "\r\n";
        ss << "\r\n"; // 空行分隔头部和正文
        ss << "<!DOCTYPE html>\r\n"
        << "<html lang=\"zh-CN\">\r\n"
        << "<head>\r\n"
        << "  <meta charset=\"UTF-8\" />\r\n"
        << "  <title>验证码</title>\r\n"
        << "</head>\r\n"
        << "<body style=\"font-family: Arial, sans-serif; font-size:14px; color:#333;\">\r\n"
        << "  <p>您好，</p>\r\n"
        << "  <p>您正在进行邮箱验证，本次验证码为：</p>\r\n"
        << "  <p style=\"font-size:20px; font-weight:bold;\">" << code << "</p>\r\n"
        << "  <p>该验证码将在 3 分钟后失效，请尽快完成验证。</p>\r\n"
        << "  <p>如果这不是您本人的操作，请忽略此邮件，无需进一步操作。</p>\r\n"
        << "  <p style=\"color:#999; font-size:12px;\">\r\n"
        << "    本邮件由系统自动发送，请不要直接回复。\r\n"
        << "  </p>\r\n"
        << "</body>\r\n"
        << "</html>\r\n";
        return ss;
    
    }
    size_t MailClient::call_back(char* ptr, size_t size, size_t nmemb, void* userdata){
        std::stringstream* ss = (std::stringstream*)userdata;
        ss->read(ptr, size * nmemb);
        return ss->gcount();
    }
}