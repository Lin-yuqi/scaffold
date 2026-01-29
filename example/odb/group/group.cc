#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/transaction.hxx>

#include "student.h"
#include "student-odb.hxx"


const std::string HOST ="192.168.86.129";
const std::string USER ="root";
const std::string PASSWD ="123456";
const std::string DBNAME ="test_odb";
const unsigned int PORT =3306;
const std::string CHARSET ="utf8";


void group_count(std::unique_ptr<odb::mysql::database>& handler){
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
}

int main(){
    // 1.构造连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(new odb::mysql::connection_pool_factory(5));
    // 2.构造数据库操作句柄
    auto handler = std::make_unique<odb::mysql::database>(USER, PASSWD, DBNAME, HOST, PORT, nullptr, CHARSET, 0,std::move(pool));
    // 3.数据操作
    group_count(handler);

    return 0;
}