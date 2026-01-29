#pragma once
#include <curl/curl.h>
#include <iostream>
#include <sstream>

namespace linmail{
    struct mail_settings{
        std::string username;
        std::string password;
        std::string url;
        std::string from;
    };

    class CodeClient{
        public:
        CodeClient()=default;
        ~CodeClient()=default;      
        virtual bool send(const std::string& to, const std::string& code)=0;
    };

    class MailClient:public CodeClient{
        public:
            //初始化全局配置，保存服务配置信息
            MailClient(const mail_settings& settings);
            //释放全局配置资源
            ~MailClient();
            //发送邮件验证码
            virtual bool send(const std::string& to, const std::string& code) override;
        private:
            std::stringstream CodeBody(const std::string& to, const std::string& code);
            static size_t call_back(char* ptr, size_t size, size_t nmemb, void* userdata);
            bool FAILED(CURLcode ret, CURL* curl);
        private:
            mail_settings _settings;
            const std::string _subject="Your Verification Code";
    };
}