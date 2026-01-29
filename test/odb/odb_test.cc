#include "../../source/odb.h"
#include <iostream>
#include "student.h"
#include "student-odb.hxx"

int main(){
    linodb::mysql_settings settings={
        .host="192.168.86.129",
        .passwd="123456",
        .db="test_odb",
        .connections_pool_size=5
    };

    auto handler=linodb::DBFactory::create_mysql_db(settings);
    odb::mysql::transaction t(handler->begin());
    try{
        typedef odb::query<Groupcount> query;
        typedef odb::result<Groupcount> result;


        result r (handler->query<Groupcount>());

        for(auto it = r.begin(); it != r.end(); ++it){
            std::cout<<"class_sn:"<<it->class_sn
                     <<", total_stu:"<<it->total_stu
                     <<", avg_score:"<<it->avg_score
                     <<std::endl;
        }

        t.commit();
    }catch(const std::exception& e){
        t.rollback();
        std::cerr<<"group_count exception:"<<e.what()<<std::endl;
    }

    return 0;
}