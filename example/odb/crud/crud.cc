#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/transaction.hxx>

#include "student.h"
#include "student-odb.hxx"


const std::string HOST ="192.168.86.129";
const std::string USER ="root";
const std::string PASSWD ="123456";
const std::string DBNAME ="test_obd";
const unsigned int PORT =3306;
const std::string CHARSET ="utf8";


void insert(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        //数据操作
        Student stu("zhangsan");
        stu.set_age(25);
        stu.set_score(95.5);
        stu.set_birth(boost::posix_time::time_from_string("2003-05-01 10:20:30"));
        db.persist(stu);
        tx.commit();//提交事务
    }catch(const odb::exception& e){
        std::cout<<e.what()<<std::endl;
    }
}

void select(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        //数据操作
        typedef odb::query<Student> Query;
        auto stu =db.query_one<Student>(Query::sn==2);
        if(!stu){
            std::cout<<"not found student"<<std::endl;
            return;
        }

        if(stu->get_age()) std::cout<<"age:"<<stu->get_age().get()<<std::endl;
        if(stu->get_score()) std::cout<<"score:"<<stu->get_score().get()<<std::endl;
        if(stu->get_birth()) std::cout<<"birth:"<<boost::posix_time::to_simple_string(stu->get_birth().get())<<std::endl;
        
        tx.commit();//提交事务
    }catch(const odb::exception& e){
        std::cout<<e.what()<<std::endl;
    }
}

void update(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        //数据操作
        typedef odb::query<Student> Query;
        auto stu =db.query_one<Student>(Query::sn==2);
        if(!stu){
            std::cout<<"not found student"<<std::endl;
            return;
        }

        stu->set_name("lisi");
        db.update(*stu);
        
        tx.commit();//提交事务
    }catch(const odb::exception& e){
        std::cout<<e.what()<<std::endl;
    }
}

void select_all(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        //数据操作
        typedef odb::query<Student> Query;
        typedef odb::result<Student> Result;

        Result r(db.query<Student>(Query::age>=18));
        for(auto it=r.begin(); it!=r.end(); ++it){
            std::cout<<"sn:"<<it->get_sn()<<", name:"<<it->get_name()<<std::endl;
        }
        
        tx.commit();//提交事务
    }catch(const odb::exception& e){
        std::cout<<e.what()<<std::endl;
    }
}

void remove(std::unique_ptr<odb::mysql::database>& handler){
    try{
        odb::transaction tx(handler->begin());
        auto& db = tx.database();
        //数据操作
        typedef odb::query<Student> Query;
        auto stu =db.query_one<Student>(Query::sn==2);
        if(!stu){
            std::cout<<"not found student"<<std::endl;
            return;
        }

        db.erase<Student>(stu->get_sn());
        
        tx.commit();//提交事务
    }catch(const odb::exception& e){
        std::cout<<e.what()<<std::endl;
    }
}

int main(){
    // 1.构造连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(new odb::mysql::connection_pool_factory(5));
    // 2.构造数据库操作句柄
    auto handler = std::make_unique<odb::mysql::database>(USER, PASSWD, DBNAME, HOST, PORT, nullptr, CHARSET, 0,std::move(pool));
    // 3.数据操作
    insert(handler);
    
    // update(handler);
    select_all(handler);

    return 0;
}