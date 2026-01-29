#pragma once
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/transaction.hxx>

namespace linodb{
    struct mysql_settings{
        std::string host;
        std::string user="root";
        std::string passwd;
        std::string db;
        std::string cset="utf8";
        unsigned int port = 3306;
        unsigned int connections_pool_size = 3;  
    };

    class DBFactory{
        public:
            static std::shared_ptr<odb::mysql::database> create_mysql_db(const mysql_settings& settings);
    };
}
