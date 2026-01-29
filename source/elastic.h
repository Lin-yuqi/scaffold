#include "util.h"
#include <cpr/response.h>
#include <elasticlient/client.h>

namespace linelastic{
    class Base{
        public:
            using ptr = std::shared_ptr<Base>;

            Base(const std::string& key);
            //想val对象中添加数据
            template<typename T>
            void add(const std::string& key, const T& value){
                _val[key] = value;
            }
            template<typename T>
            void append(const std::string& key, const T& value){
                _val[key].append(value);
            }

            virtual std::string to_string()const;
            virtual const Json::Value val()const;
            const std::string& key()const;
            
        protected:
            std::string _key;
            Json::Value _val;
    };
    class Array;
    class Object:public Base{
        public:
            using ptr = std::shared_ptr<Object>;
            Object(const std::string& key);
            void addElement(const Base::ptr& sub);
            virtual std::string to_string()const override;
            virtual const Json::Value val()const override;
            template<typename R>
            std::shared_ptr<R> getElement(const std::string& key)const{
                if(_subs.find(key)!=_subs.end()){
                    return std::dynamic_pointer_cast<R>(_subs.at(key));
                }
                return nullptr;
            }

            std::shared_ptr<Object> newObject(const std::string& key);
            std::shared_ptr<Array> newArray(const std::string& key);
        private:
            std::unordered_map<std::string, Base::ptr> _subs;
    };

    class Array:public Base{
        public:
            using ptr = std::shared_ptr<Array>;
            Array(const std::string& key);
            void addElement(const Base::ptr& sub);
            virtual std::string to_string()const override;
            virtual const Json::Value val()const override;
            std::shared_ptr<Object> newObject(const std::string& key);
            std::shared_ptr<Array> newArray(const std::string& key);
        private:
            std::vector<Base::ptr> _subs;  
    };

    class Tokenizer:public Object{
        public:
            using ptr = std::shared_ptr<Tokenizer>;
            Tokenizer(const std::string& key);
            void tokenizer(const std::string& word);
            void type(const std::string& type);
    };

    class Analyzer:public Object{
        public:
            using ptr = std::shared_ptr<Analyzer>;
            Analyzer();
            Tokenizer::ptr tokenizer(const std::string& name);
    };

    class Analysis:public Object{
        public:
            using ptr = std::shared_ptr<Analysis>;
            Analysis();
            Analyzer::ptr analyzer();
    };

    class Settings:public Object{
        public:
            using ptr = std::shared_ptr<Settings>;
            Settings();
            Analysis::ptr analysis();
    };

    class Field:public Object{
        public:
            using ptr = std::shared_ptr<Field>;
            Field(const std::string& name);
            void type(const std::string& type);
            void boost(double boost);
            void index(bool flag);
            void analyzer(const std::string& analyzer);
    };

    class Properties:public Object{
        public:
            using ptr = std::shared_ptr<Properties>;
            Properties();
            Field::ptr field(const std::string& name);
    };

    class Mappings:public Object{
        public:
            using ptr = std::shared_ptr<Mappings>;
            Mappings();
            Properties::ptr properties();
            void dynamic(bool flag);
    };

    class Term :public Object{
        public:
            using ptr = std::shared_ptr<Term>;
            Term(const std::string& field);
            template<typename T>
            void setValue(const T& value){
                this->add(_field, value);
            }
        private:
            std::string _field;
    };

    class Terms :public Object{
        public:
            using ptr = std::shared_ptr<Terms>;
            Terms(const std::string& field);
            template<typename T>
            void setValue(const T& value){
                this->append(_field, value);
            }
        private:
            std::string _field;
    };

    class Match :public Object{
        public:
            using ptr = std::shared_ptr<Match>;
            Match(const std::string& field);
            template<typename T>
            void setValue(const T& value){
                this->add(_field, value);
            }
        private:
            std::string _field;
    };

    class Multi_match :public Object{
        public:
            using ptr = std::shared_ptr<Multi_match>;
            Multi_match();
            void appendField(const std::string& field);
            template<typename T>
            void setQuery(const T& query){
                this->add("query", query); 
            }
    };

    class Range :public Object{
        public:
            using ptr = std::shared_ptr<Range>;
            Range(const std::string& field);
            template<typename T>
            void setRange(const T& lt, const T& gt){
                _sub->add("gt", lt);
                _sub->add("lt", gt);
            }
        private:
            Object::ptr _sub;
    };

    class Qobject:public Object{
        public:
            using ptr = std::shared_ptr<Qobject>;
            Qobject(const std::string& key);
            Term::ptr term(const std::string& field);
            Terms::ptr terms(const std::string& field);
            Match::ptr match(const std::string& field);
            Multi_match::ptr multi_match();
            Range::ptr range(const std::string& field);

    };

    class Qarray:public Array{
        public:
            using ptr = std::shared_ptr<Qarray>;
            Qarray(const std::string& key);
            Term::ptr term(const std::string& field);
            Terms::ptr terms(const std::string& field);
            Match::ptr match(const std::string& field);
            Multi_match::ptr multi_match();
            Range::ptr range(const std::string& field);
    };

    class Must:public Qarray{
        public:
            using ptr = std::shared_ptr<Must>;
            Must();
    };

    class Should:public Qarray{
        public:
            using ptr = std::shared_ptr<Should>;
            Should();
    };

    class MustNot:public Qarray{
        public:
            using ptr = std::shared_ptr<MustNot>;
            MustNot();
    };

    class QBool:public Qobject{
        public:
            using ptr = std::shared_ptr<QBool>;
            QBool();
            Must::ptr must();
            Should::ptr should();
            MustNot::ptr must_not();
            void minimum_should_match(size_t value);
    };
        
    class Query:public Qobject{
        public:
            using ptr = std::shared_ptr<Query>;
            Query();
            void match_all();
            QBool::ptr qbool();
            Must::ptr must();
            Should::ptr should();
            MustNot::ptr must_not();
    };

    class Request{
        public:
            Request(const std::string& index, const std::string& type, const std::string& op, const std::string& id);
            void set_index(const std::string& index);
            void set_type(const std::string& type);
            void set_id(const std::string& id);
            void set_op(const std::string& op);
            const std::string& index()const;
            const std::string& type()const;
            const std::string& op()const;
            const std::string& id()const;
        protected:
            std::string _index;
            std::string _type;
            std::string _op;
            std::string _id;
    };

    class Indexer:public Object, public Request{
        public:
            using ptr = std::shared_ptr<Indexer>;
            Indexer(const std::string& index);
            Settings::ptr settings();
            Tokenizer::ptr tokenizer(const std::string& name);
            Mappings::ptr mappings();
            Field::ptr field(const std::string& name);
    };

    class Inserter:public Object, public Request{
        public:
            using ptr = std::shared_ptr<Inserter>;
            Inserter(const std::string& index, const std::string& id);
    };

    class Updater:public Object, public Request{
        public:
            using ptr = std::shared_ptr<Updater>;
            Updater(const std::string& index, const std::string& id);
            Object::ptr doc();
            template<typename T>
            void add(const std::string& key, const T& value){
                this->doc()->add(key, value);
            }
            template<typename T>
            void append(const std::string& key, const T& value){
                this->doc()->append(key, value);
            }
    };

    class Deleter:public Object, public Request{
        public:
            using ptr = std::shared_ptr<Deleter>;
            Deleter(const std::string& index, const std::string& id);
    };

    class Searcher:public Object, public Request{
        public:
            using ptr = std::shared_ptr<Searcher>;
            Searcher(const std::string& index);
            Query::ptr query();
            void size(size_t sz);
            void from(size_t fr);
    };

    class BaseClient{
        public:
            BaseClient()=default;
            virtual ~BaseClient()=default;
            virtual bool create(const Indexer& indx)=0;
            virtual bool insert(const Inserter& ins)=0;
            virtual bool update(const Updater& upd)=0;
            virtual bool remove(const Deleter& del)=0;
            virtual bool remove(const std::string& index)=0;
            virtual std::optional<Json::Value> search(const Searcher& sea)=0;
    };

    class ESClient:public BaseClient{
        public:
            using ptr =  std::shared_ptr<ESClient>;
            ESClient(const std::vector<std::string>& hosts);
            bool create(const Indexer& indx)override;
            bool insert(const Inserter& ins)override;
            bool update(const Updater& upd)override;
            bool remove(const Deleter& del)override;
            bool remove(const std::string& index)override;
            std::optional<Json::Value> search(const Searcher& sea)override;
        private:
            std::shared_ptr<elasticlient::Client> _client;
    };
}