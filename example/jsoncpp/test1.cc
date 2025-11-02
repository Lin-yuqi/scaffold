#include <jsoncpp/json/json.h>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>

int main(){
    Json::Value root;
    root["name"] = "John Doe";
    root["age"] = 30;
    root["scores"].append(85.3);
    root["scores"].append(90.5);
    root["scores"].append(78.9);

    std::unique_ptr<Json::StreamWriter> writer(Json::StreamWriterBuilder().newStreamWriter());
    std::stringstream ss;
    int ret = writer->write(root, &ss);
    if(ret != 0){
        std::cerr << "Error writing JSON" << std::endl;
        return ret;
    }
    std::cout << ss.str() << std::endl;

    return 0;
}