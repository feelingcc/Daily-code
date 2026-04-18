#pragma once

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>

namespace verification_code {
    struct MailSetting {
        std::string username;
        std::string password;
        std::string url;
        std::string from;
    };

    class CodeClient {
        public:
            CodeClient() = default;
            virtual ~CodeClient() = default;
            virtual bool send(const std::string& to , const std::string& code) = 0;
    };

    class MailClient : public CodeClient {
        public:
            MailClient(const MailSetting& setting , const std::string& title = "验证码") 
                :_title(title)
                ,_setting(setting)
            {
                // 初始化全局配置
                CURLcode global_init_code = curl_global_init(CURL_GLOBAL_DEFAULT);
                if(global_init_code != CURLcode::CURLE_OK) {
                    std::cerr << "初始化CURL全局配置失败: " << curl_easy_strerror(global_init_code) << std::endl;
                    abort();
                }
            }

            ~MailClient() {
                // 销毁全局配置
                curl_global_cleanup();
            }

            void setTitle(const std::string& title) { _title = title; }

            // 发送验证码
            virtual bool send(const std::string& to , const std::string& code) {
                // 1. 创建并初始化 curl 操作句柄
                CURL* curl = curl_easy_init();
                // 2. 设置 CURL 的 url 配置
                if (!set_curl_option(curl , CURLOPT_URL , _setting.url.c_str())) return false;
                // 3. 设置 CURL 的用户名
                if (!set_curl_option(curl , CURLOPT_USERNAME , _setting.username.c_str())) return false;
                // 4. 设置 CURL 的授权码
                if (!set_curl_option(curl , CURLOPT_PASSWORD , _setting.password.c_str())) return false;
                // 4. 设置 CURL 的发件人
                if (!set_curl_option(curl , CURLOPT_MAIL_FROM , _setting.from.c_str())) return false;
                // 5. 设置 CURL 的收件人
                struct curl_slist* to_lists = nullptr;
                to_lists = curl_slist_append(to_lists , to.c_str());
                if (!set_curl_option(curl , CURLOPT_MAIL_RCPT , to_lists)) return false;
                // 6. 设置 CURL 的通信正文
                std::stringstream content = body(code);
                if (!set_curl_option(curl , CURLOPT_READDATA , static_cast<void*>(&content))) return false;
                // 7. 设置 CURL 的处理请求数据的回调函数
                if (!set_curl_option(curl , CURLOPT_READFUNCTION , handleCallback)) return false;
                // 8. 设置 CURL 的上传模式
                if (!set_curl_option(curl , CURLOPT_UPLOAD , 1L)) return false;
                // 9. 发送请求
                CURLcode perform_code = curl_easy_perform(curl);
                if (perform_code != CURLcode::CURLE_OK) {
                    std::cerr << "请求邮件服务器失败: " << curl_easy_strerror(perform_code) << std::endl;
                    return false;
                }
                // 10. 释放 curl_slist 资源
                curl_slist_free_all(to_lists);
                // 11. 释放 curl 操作句柄
                curl_easy_cleanup(curl);
                std::cout << "发送邮件验证码成功: " << to << "-" << code << std::endl;
                return true; 
            }
        private:
            template <typename T>
            bool set_curl_option(CURL* curl, CURLoption option , const T& value) {
                CURLcode code = curl_easy_setopt(curl , option , value);
                if(code != CURLcode::CURLE_OK) {
                    std::cerr << "设置CURL配置失败: " << curl_easy_strerror(code) << std::endl;
                    return false;
                }   
                return true;
            }

            std::stringstream body(const std::string& code) {
                std::stringstream html;
                html << "Subject: " << _title << "\r\n";
                html << "Content-Type: text/html\r\n";
                html << "\r\n";
                html << "<html><body><div style='padding: 20px; font-family: Arial;'><h3>验证码: " << code << "</h3><p style='color: #999;'>5分钟内有效</p></div></body></html>" << "\r\n";
                return html;
            }

            static size_t handleCallback(char *buffer, size_t size, size_t nitems, void *userdata) {
                std::stringstream* ss = static_cast<std::stringstream*>(userdata);
                ss->read(buffer, size * nitems);
                return ss->gcount();
            }
        private:
            std::string _title;
            MailSetting _setting;
    };
}