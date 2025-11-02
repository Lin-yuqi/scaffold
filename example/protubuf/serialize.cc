/*
    Protobuf序列化示例
    1.编写接口描述语言，进行数据结构描述
    2.编译proto文件，生成对应的代码文件
    3.包含头文件
    4.进行序列化和反序列化操作
*/

#include "person.pb.h"
#include <iostream>

std::string serialize(){
    person::Person ps;
    ps.set_id(1);
    ps.set_name("Alice");
    ps.set_sex(person::gender::female);
    auto scores = ps.mutable_scores();
    scores->Add(95.5);
    scores->Add(88.0);
    auto tags = ps.mutable_tags();
    (*tags)[1] = "tag1";
    (*tags)[2] = "tag2";

    std::string output;
    return ps.SerializeAsString();
}

void unserialize(const std::string& data){
    person::Person ps;
    if(ps.ParseFromString(data)){
        std::cout << "ID: " << ps.id() << std::endl;
        std::cout << "Name: " << ps.name() << std::endl;    
        std::cout << "sex: " << ps.sex() << std::endl;
        std::cout << "Scores: ";
        for(int i = 0; i < ps.scores_size(); ++i){
            std::cout << ps.scores(i) << " ";
        }
        std::cout << std::endl;
        std::cout << "Tags: ";
        for(const auto& tag : ps.tags()){
            std::cout << "{" << tag.first << ": " << tag.second << "} ";
        }
        std::cout << std::endl;
    }

    return;
}

int main(){
    std::string data = serialize();
    std::cout << "Serialized data: " << data << std::endl;
    std::cout << "Unserialized data: " << std::endl;
    unserialize(data);

    return 0;
}