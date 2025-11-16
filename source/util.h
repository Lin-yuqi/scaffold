/*
    util工具封装

*/
#pragma once
#include <jsoncpp/json/json.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <optional>
#include "log.h"




namespace linutil{
    class JSON{
        public:
            static std::optional<std::string> serialize(const Json::Value &value);
            static std::optional<Json::Value> unserialize(const std::string &str); 
    };

    class FUTIL { 
        public:
            static bool read(const std::string &filename, std::string &body);
            static bool write(const std::string &filename, const std::string &body);
    };

    class STR {
        public:
            static size_t split(const std::string &src, const std::string &sep, std::vector<std::string> &dst);
    };

    const size_t UUID_LEN = 16;
    enum class RandType{
        CHAR,
        DIGIT,
        MIX
    };
    class Random {
        public:
            static std::string code(size_t len = UUID_LEN,RandType type = RandType::MIX);
    };

}