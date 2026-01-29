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

        size_t sn() const { return _sn; }
        void set_sn(size_t sn) { _sn = sn; }
        size_t classes_id() const { return _classes_id; }
        void set_classes_id(size_t classes_id) { _classes_id = classes_id; }
        std::string name() const { return _name; }
        void set_name(const std::string& name) { _name = name; }
        odb::nullable<int> age() const { return _age; }
        void set_age(const odb::nullable<int>& age) { _age = age; }
        odb::nullable<double> score() const { return _score; }
        void set_score(const odb::nullable<double>& score) { _score = score; }
        odb::nullable<boost::posix_time::ptime> birth() const { return _birth; }
        void set_birth(const odb::nullable<boost::posix_time::ptime>& birth) { _birth = birth; }

    private:
        friend class odb::access;
        #pragma db id auto
        size_t _sn;
        #pragma db index
        size_t _classes_id;
        std::string _name;
        odb::nullable<int> _age;
        odb::nullable<double> _score;
        odb::nullable<boost::posix_time::ptime> _birth;
};

#pragma db object table("tb_classes")
class Classes{
    public:
        Classes(){}
        Classes(const std::string& name): _name(name){}
        size_t sn() const { return _sn; }
        void set_sn(size_t sn) { _sn = sn; }
        std::string name() const { return _name; }
        void set_name(const std::string& name) { _name = name; }

    private:
        friend class odb::access;
        #pragma db id auto
        size_t _sn;
        #pragma db unique type("VARCHAR(64)")
        std::string _name;
};

#pragma db view object(Student) \
                query((?)+" GROUP BY "+Student::_classes_id)

struct Groupcount{
    #pragma db column(Student::_classes_id)
    size_t class_sn;
    #pragma db column("count("+Student::_sn+")")
    size_t total_stu;
    #pragma db column("avg("+Student::_score+")")
    double avg_score;
};