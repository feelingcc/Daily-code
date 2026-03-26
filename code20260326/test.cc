#include "util.h"

int main() {

    // 序列化测试
    Json::Value root;
    root["name"] = "张三";
    root["age"] = 25;
    root["address"]["city"] = "北京";   
    root["score"].append(10);      
    root["score"].append(20.5);
    std::optional<std::string> json_str = util::JSON::serialize(root);
    if(json_str) {
        std::cout << "序列化结果\n" << json_str.value() << std::endl;
    }

    // 反序列化测试
    std::string str = R"({
        "address":{
            "city":"Anytown",
            "state":"CA",
            "street":"123 Main St"
        },
        "age":30,
        "is_student":false,
        "name":"John Doe"
    })";
    std::optional<Json::Value> val =  util::JSON::unserialize(str);
    if(val) {
        std::cout << "反序列化结果\n";
        std::cout << val.value()["address"]["city"].asString() << std::endl;
        std::cout << val.value()["address"]["state"].asString() << std::endl;
        std::cout << val.value()["address"]["street"].asString() << std::endl;
        std::cout << val.value()["age"].asInt() << std::endl;
        std::cout << val.value()["name"].asString() << std::endl;
        std::cout << val.value()["is_student"].asBool() << std::endl;
    }

    return 0;
}