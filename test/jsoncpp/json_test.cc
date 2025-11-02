#include "../../source/util.h"
#include <iostream>
void serialize_test(){
    Json::Value root;
    root["name"] = "example";
    root["value"] = 42;
    root["scores"] = Json::arrayValue;
    root["scores"].append(95);  
    root["scores"].append(88);
    
    auto ret = linutil::JSON::serialize(root);
    std::cout << "Serialized JSON: " << *ret << std::endl;
    if(!ret){
        return;
    }

}

void unserialize_test(){
    std::string json_str = R"({"name":"example","value":42,"scores":[95,88]})";
    auto root = linutil::JSON::unserialize(json_str);
    if(!root){
        std::cerr << "Failed to unserialize JSON" << std::endl;
        return;
    }
    std::cout << "Unserialized JSON name: " << (*root)["name"].asString() << std::endl;
    std::cout << "Unserialized JSON value: " << (*root)["value"].asInt() << std::endl;
    std::cout << "Unserialized JSON scores: ";
    for(const auto& score : (*root)["scores"]){
        std::cout << score.asInt() << " ";
    }
    std::cout << std::endl;
}

int main(){
    serialize_test();
    unserialize_test();
    return 0;
}
