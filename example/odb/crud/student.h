#pragma once
#include <string>
#include <cstddef>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <odb/core.hxx>
#include <odb/nullable.hxx>

#pragma db object table("tb_student")
class Student{
    public:
        Student(){}
        Student(const std::string& name): _name(name){}

        size_t get_sn() const { return _sn; }
        void set_sn(size_t sn) { _sn = sn; }
        std::string get_name() const { return _name; }
        void set_name(const std::string& name) { _name = name; }
        odb::nullable<int> get_age() const { return _age; }
        void set_age(const odb::nullable<int>& age) { _age = age; }
        odb::nullable<double> get_score() const { return _score; }
        void set_score(const odb::nullable<double>& score) { _score = score; }
        odb::nullable<boost::posix_time::ptime> get_birth() const { return _birth; }
        void set_birth(const odb::nullable<boost::posix_time::ptime>& birth) { _birth = birth; }

    private:
        friend class odb::access;
        #pragma db id auto
        size_t _sn;
        std::string _name;
        odb::nullable<int> _age;
        odb::nullable<double> _score;
        odb::nullable<boost::posix_time::ptime> _birth;
};