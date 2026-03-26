#pragma once

#include <jsoncpp/json/json.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <optional>

namespace util {
    // JSON 而不是 Json 防止和 jsoncpp 中的重名
    struct JSON {
        // 注意，函数实现时，无需加 static 和 缺省参数
        static std::optional<std::string> serialize(const Json::Value& value , bool style = false);
        static std::optional<Json::Value> unserialize(const std::string& input);
    };
}