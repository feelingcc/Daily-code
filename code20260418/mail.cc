#include "verification_code.hpp"
#include <memory>

int main() {

    verification_code::MailSetting mail_setting = {
        .username = "foreverdong7@163.com",
        .password = "YXdj5j6cBg8JaRRi",
        .url = "smtps://smtp.163.com:465",
        .from = "foreverdong7@163.com"
    };
    std::shared_ptr<verification_code::CodeClient> code_client = std::make_shared<verification_code::MailClient>(mail_setting);
    bool res = code_client->send("3027738783@qq.com" , "1234");
    if(!res) {
        std::cout << "发送邮件失败！" << std::endl;
    }

    return 0;
}