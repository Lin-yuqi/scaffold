#include "util.h"
#include "log.h"
#include <iostream>


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
        ERR("反序列化失败: {}", err.c_str());
        return std::nullopt;
    }
    return root;
}

bool FUTIL::read(const std::string &filename, std::string &body) {
    std::ifstream ifs;
    ifs.open(filename, std::ios::in|std::ios::binary);
    if(!ifs.is_open()){
        ERR("文件打开失败: {}", filename.c_str());
        return false;   
    }

    size_t fsz;
    ifs.seekg(0, std::ios::end);
    fsz = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    body.resize(fsz);
    ifs.read(&body[0], fsz);
    if(ifs.good()==false){
        ERR("文件读取失败: {}", filename.c_str());
        ifs.close();
        return false;   
    }
    ifs.close();
    return true;
}

bool FUTIL::write(const std::string &filename, const std::string &body){
    std::ofstream ofs;
    ofs.open(filename, std::ios::out|std::ios::binary|std::ios::trunc);
    if(!ofs.is_open()){
        ERR("文件打开失败: {}", filename.c_str());
        return false;
    }

    ofs.write(body.c_str(), body.size());

    if(!ofs.good()){
        ERR("文件写入失败: {}", filename.c_str());
        ofs.close();
        return false;
    }

    ofs.close();
    return true;
}

size_t STR::split(const std::string &src, const std::string &sep, std::vector<std::string> &dst) {
    size_t pos,idx=0;
    while(1){
        pos=src.find(sep,idx);
        if(pos==std::string::npos){
            break;
        }
        if(pos==idx){
            idx=pos+sep.size();
            continue;
        }
        dst.push_back(src.substr(idx,pos-idx));
        idx=pos+sep.size();
    }
    if(idx<src.size()){
        dst.push_back(src.substr(idx,src.size()-idx));
    }
    return dst.size();
}


}