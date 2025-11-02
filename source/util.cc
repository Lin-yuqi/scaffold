#include "util.h"
#include "linlog.h"

namespace linutil{
std::optional<std::string> JSON::serialize(const Json::Value &value){
    std::unique_ptr<Json::StreamWriter> writer(Json::StreamWriterBuilder().newStreamWriter());
    std::stringstream ss;
    int ret = writer->write(value, &ss);
    if (ret != 0) {
        ERR("序列化失败");
        return std::nullopt;
    }
    std::string result = ss.str();
    return result;
}

std::optional<Json::Value> JSON::unserialize(const std::string &str){
    std::unique_ptr<Json::CharReader> reader(Json::CharReaderBuilder().newCharReader());
    Json::Value root;
    std::string err;
    bool ret=reader->parse(str.c_str(), str.c_str()+str.size(), &root, &err);
    if(!ret){
        ERR("反序列化失败: %s", err.c_str());
        return std::nullopt;
    }
    return root;
}
}