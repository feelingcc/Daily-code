#include <jsoncpp/json/json.h>
#include <iostream>
#include <sstream>

int main() {

    Json::Value root;
    // 添加元素
    root["name"] = "张三";
    root["age"] = 25;
    root["address"]["city"] = "北京";   // 自动创建 address 对象
    root["score"].append(10);      // 自动创建 score 数组
    root["score"].append(20.5);

    // 序列化
    Json::StreamWriterBuilder writeBuilder;
    writeBuilder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> writer(writeBuilder.newStreamWriter());
    std::stringstream ss;
    writer->write(root, &ss);
    std::cout << "序列化：" << std::endl;
    std::cout << ss.str() << std::endl;
    // 反序列化
    std::string json_str = ss.str();
    Json::CharReaderBuilder readBuilder;
    std::unique_ptr<Json::CharReader> reader(readBuilder.newCharReader());
    std::string err;
    Json::Value res;
    reader->parse(json_str.c_str() , json_str.c_str() + json_str.size() , &res , &err);
    std::cout << "反序列化：" << std::endl;
    std::cout << res["name"].asString() << std::endl;
    std::cout << res["age"].asInt() << std::endl;
    if(!res["address"].isNull() && res["address"].isObject())
        std::cout << res["address"]["city"].asString() << std::endl;
    if(!res["score"].isNull() && res["score"].isArray()){
        size_t n = res["score"].size();
        for(int i = 0; i < n; i++)
            std::cout << res["score"][i] << std::endl;
    }


    return 0;
}

// int main() {

//     Json::Value root;
//     // 添加元素
//     root["name"] = "张三";
//     root["age"] = 25;
//     root["address"]["city"] = "北京";   // 自动创建 address 对象
//     root["score"].append(10);      // 自动创建 score 数组
//     root["score"].append(20.5);
//     std::cout << root.toStyledString() << std::endl;

//     // 访问元素
//     std::cout << root["name"].asString() << std::endl;
//     std::cout << root["age"].asInt() << std::endl;
//     std::cout << root["address"]["city"].asString() << std::endl;
//     // 访问数组元素
//     size_t n = root["score"].size();
//     for(int i = 0; i < n; i++)
//         std::cout << root["score"][i] << std::endl;

//     return 0;
// }