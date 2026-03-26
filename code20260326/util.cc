#include "util.h"

namespace util {
    
    // 定义不需要 static 关键字
    std::optional<std::string> JSON::serialize(const Json::Value& value , bool style) {
        Json::StreamWriterBuilder writeBuilder;
        if(style) {
            writeBuilder["emitUTF8"] = true;
            writeBuilder["indentation"] = " ";
        } else {
            writeBuilder["emitUTF8"] = false;
            writeBuilder["indentation"] = "";
        }
        std::unique_ptr<Json::StreamWriter> writer(writeBuilder.newStreamWriter());
        std::stringstream ss;
        if(writer->write(value, &ss)) {
            // 序列化失败
            std::cout << "序列化失败！" << std::endl;
            return std::nullopt;
        }
        return ss.str();
    }   

    std::optional<Json::Value> JSON::unserialize(const std::string& input) {
        Json::CharReaderBuilder readBuilder;
        std::unique_ptr<Json::CharReader> reader(readBuilder.newCharReader());
        std::string err;
        Json::Value res;
        if(!reader->parse(input.c_str() , input.c_str() + input.size() , &res , &err)) {
            // 反序列化失败
            std::cout << input << " 序列化失败: " << err << std::endl;
            return std::nullopt;
        }
        return res;
    }

}