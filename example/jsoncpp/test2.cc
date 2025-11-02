#include <jsoncpp/json/json.h>
#include <iostream>
#include <string>
#include <memory>

int main(){
    std::string str=R"({
        "age" : 30,
        "name" : "John Doe",
        "scores" : 
        [
                85.299999999999997,
                90.5,
                78.900000000000006
        ]
})";
    Json::CharReaderBuilder readerBuilder;
    std::unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
    Json::Value root;
    std::string err;
    bool ret=reader->parse(str.c_str(), str.c_str()+str.size(), &root, &err);
    if(!ret){
        std::cerr<<"Error parsing JSON: "<<err<<std::endl;
        return -1;
    }
    std::cout<<"Name: "<<root["name"].asString()<<std::endl;
    std::cout<<"Age: "<<root["age"].asInt()<<std::endl;
    std::cout<<"Scores: ";
    for(const auto& score : root["scores"]){
        std::cout<<score.asDouble()<<" ";
    }
    std::cout<<std::endl;
    return 0;
}
