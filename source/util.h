/*
    util工具封装

*/
#pragma once
#include <string>
#include <optional>
#include <jsoncpp/json/json.h>


namespace linutil{
    class JSON{
        public:
            static std::optional<std::string> serialize(const Json::Value &value);
            static std::optional<Json::Value> unserialize(const std::string &str); 
    };

}