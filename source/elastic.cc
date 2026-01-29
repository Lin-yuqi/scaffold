#include "elastic.h"
#include "util.h"
#include "log.h"

namespace linelastic{
    Base::Base(const std::string& key):_key(key){}
    std::string Base::to_string()const{
        Json::Value root;
        root[_key] = _val;
        return *linutil::JSON::serialize(root,true);
    }
    const std::string& Base::key()const{return _key;}
    const Json::Value Base::val()const{return _val;}

    Object::Object(const std::string& key):Base(key){}

    void Object::addElement(const Base::ptr& sub){
        _subs[sub->key()] = sub;
    }

    std::shared_ptr<Object> Object::newObject(const std::string& key){
        std::shared_ptr<Object> obj = this->getElement<Object>(key);
        if(obj){
            return obj;
        }
        obj = std::make_shared<Object>(key);
        this->addElement(obj);
        return obj;
    }

    std::shared_ptr<Array> Object::newArray(const std::string& key){
        std::shared_ptr<Array> arr = this->getElement<Array>(key);
        if(arr){
            return arr;
        }
        arr = std::make_shared<Array>(key);
        this->addElement(arr);
        return arr;
    }

    std::string Object::to_string()const{
        Json::Value root=_val;
        for(const auto& [k, v]:_subs){
            root[k]=v->val();
        }
        return *linutil::JSON::serialize(root,true);
    }
    

    const Json::Value Object::val()const{
        Json::Value root = _val;//保持父类的val内容
        for(const auto& [k, v]:_subs){
            root[k]=v->val();
        }
        return root;
    }

    Array::Array(const std::string& key):Base(key){}
    void Array::addElement(const Base::ptr& sub){
        _subs.push_back(sub);
    }
    std::string Array::to_string()const{
        Json::Value root = _val;
        for (auto &i: _subs) {
            root.append(i->val());
        }
        return *linutil::JSON::serialize(root,true);
    }
    const Json::Value Array::val()const{
        Json::Value root = _val;
        for (auto &i: _subs) {
            root.append(i->val());
        }
        return root;
    }
    std::shared_ptr<Object> Array::newObject(const std::string& key){
        auto sub=std::make_shared<Object>(key);
        this->addElement(sub);
        return sub;
    }
    std::shared_ptr<Array> Array::newArray(const std::string& key){
        auto sub=std::make_shared<Array>(key);
        this->addElement(sub);
        return sub;
    }

    Tokenizer::Tokenizer(const std::string& key):Object(key){
        this->tokenizer("ik_max_word");
        this->type("custom");
    }
    void Tokenizer::tokenizer(const std::string& word){
        this->add("tokenizer", word);
    }
    void Tokenizer::type(const std::string& type){
        this->add("type", type);
    }


    Analyzer::Analyzer():Object("analyzer"){}
    Tokenizer::ptr Analyzer::tokenizer(const std::string& name){
        auto tk= this->getElement<Tokenizer>(name);
        if(tk){
            return tk;  
        }
        tk = std::make_shared<Tokenizer>(name);
        this->addElement(tk);
        return tk;
    }

    Analysis::Analysis():Object("analysis"){}
    Analyzer::ptr Analysis::analyzer(){
        auto an= this->getElement<Analyzer>("analyzer");
        if(an){
            return an;
        }
        an = std::make_shared<Analyzer>();
        this->addElement(an);
        return an;
    }

    Settings::Settings():Object("settings"){}
    Analysis::ptr Settings::analysis(){
        Analysis::ptr an;
        an=this->getElement<Analysis>("analysis");
        if(an){
            return an;
        }
        an = std::make_shared<Analysis>();
        this->addElement(an);
        return an;
    }

    Field::Field(const std::string& name):Object(name){
            this->type("text");
    }
    void Field::type(const std::string& type){
        this->add("type", type);
    }
    void Field::boost(double boost){
        this->add("boost", boost);
    }
    void Field::index(bool flag){
        this->add("index", flag);
    }
    void Field::analyzer(const std::string& analyzer){
        this->add("analyzer", analyzer);
    }

    Properties::Properties():Object("properties"){}
    Field::ptr Properties::field(const std::string& name){
        Field::ptr fd;
        fd=this->getElement<Field>(name);
        if(fd){
            return fd;  
        }
        fd = std::make_shared<Field>(name);
        this->addElement(fd);   
        return fd;
    }
    
    Mappings::Mappings():Object("mapping"){
        this->dynamic(false);
    }
    Properties::ptr Mappings::properties(){
        Properties::ptr ps;
        ps=this->getElement<Properties>("properties");
        if(ps){
            return ps;
        }
        ps = std::make_shared<Properties>();
        this->addElement(ps);
        return ps;
    }

    void Mappings::dynamic(bool flag){
        this->add("dynamic", flag);
    }


    Term::Term(const std::string& field):Object("term"), _field(field){}
    Terms::Terms(const std::string& field):Object("terms"), _field(field){}
    Match::Match(const std::string& field):Object("match"), _field(field){}
    Multi_match::Multi_match():Object("multi_match"){}
    Range::Range(const std::string& field):Object("range"){
        _sub = std::make_shared<Object>(field);
        this->addElement(_sub);
    }

    void Multi_match::appendField(const std::string& field){
        this->append("fields", field);
    }

    Qobject::Qobject(const std::string& key):Object(key){}

    Term::ptr Qobject::term(const std::string& field){
        Term::ptr term = this->getElement<Term>("term");
        if(term){
            return term;
        }
        term = std::make_shared<Term>(field);
        this->addElement(term);
        return term;
    }
    Terms::ptr Qobject::terms(const std::string& field){
        Terms::ptr terms = this->getElement<Terms>("terms");
        if(terms){
            return terms;
        }
        terms = std::make_shared<Terms>(field);
        this->addElement(terms);
        return terms;
    }
    Match::ptr Qobject::match(const std::string& field){
        Match::ptr match = this->getElement<Match>("match");
        if(match){
            return match;
        }   
        match = std::make_shared<Match>(field);
        this->addElement(match);
        return match;
    }
    Multi_match::ptr Qobject::multi_match(){
        Multi_match::ptr multi_match = this->getElement<Multi_match>("multi_match");
        if(multi_match){
            return multi_match;
        }
        multi_match = std::make_shared<Multi_match>();
        this->addElement(multi_match);
        return multi_match;
    }
    Range::ptr Qobject::range(const std::string& field){
        Range::ptr range = this->getElement<Range>("range");
        if(range){
            return range;
        }
        range = std::make_shared<Range>(field);
        this->addElement(range);
        return range;
    }

    Qarray::Qarray(const std::string& key):Array(key){} 
    Term::ptr Qarray::term(const std::string& field){
        auto obj = this->newObject("");
        Term::ptr term = std::make_shared<Term>(field);
        obj->addElement(term);
        return term;
    }

    Must::Must():Qarray("must"){}
    Should::Should():Qarray("should"){}
    MustNot::MustNot():Qarray("must_not"){}

    QBool::QBool():Qobject("bool"){}
    void QBool::minimum_should_match(size_t value){
        this->add("minimum_should_match", value);
    }
    Must::ptr QBool::must(){
        Must::ptr mst;
        mst=this->getElement<Must>("must");
        if(mst){
            return mst;
        }
        mst = std::make_shared<Must>();
        this->addElement(mst);
        return mst;
    }
    Should::ptr QBool::should(){
        Should::ptr shld;
        shld=this->getElement<Should>("should");
        if(shld){
            return shld;
        }
        shld = std::make_shared<Should>();
        this->addElement(shld);
        return shld;
    }
    MustNot::ptr QBool::must_not(){
        MustNot::ptr must_not;
        must_not=this->getElement<MustNot>("must_not");
        if(must_not){
            return must_not;
        }
        must_not = std::make_shared<MustNot>();
        this->addElement(must_not);
        return must_not;
    }   
    
    Terms::ptr Qarray::terms(const std::string& field){
        auto obj = this->newObject("");
        Terms::ptr terms = std::make_shared<Terms>(field);
        obj->addElement(terms);
        return terms;
    }
    Match::ptr Qarray::match(const std::string& field){
        auto obj = this->newObject("");
        Match::ptr match = std::make_shared<Match>(field);
        obj->addElement(match);
        return match;
    }
    Multi_match::ptr Qarray::multi_match(){
        auto obj = this->newObject("");
        Multi_match::ptr multi_match = std::make_shared<Multi_match>();
        obj->addElement(multi_match);
        return multi_match;
    }
    Range::ptr Qarray::range(const std::string& field){
        auto obj = this->newObject("");
        Range::ptr range = std::make_shared<Range>(field);
        obj->addElement(range);
        return range;
    }

    Query::Query():Qobject("query"){}
    void Query::match_all(){
        this->add("match_all", Json::Value(Json::ValueType::objectValue));
    }

    QBool::ptr Query::qbool(){
        QBool::ptr qb;
        qb=this->getElement<QBool>("bool");
        if(qb){
            return qb;
        }
        qb = std::make_shared<QBool>()  ;
        this->addElement(qb);
        return qb;
    }

    Must::ptr Query::must(){
        return this->qbool()->must();
    }
    Should::ptr Query::should(){
        return this->qbool()->should();
    }
    MustNot::ptr Query::must_not(){
        return this->qbool()->must_not();
    }

    Request::Request(const std::string& index, const std::string& type, const std::string& op, const std::string& id)
        :_index(index), _type(type), _op(op), _id(id){}
    void Request::set_index(const std::string& index){ _index = index; }
    void Request::set_type(const std::string& type){ _type = type; }
    void Request::set_id(const std::string& id){ _id = id; }
    void Request::set_op(const std::string& op){ _op = op; }
    const std::string& Request::index()const{ return _index; }
    const std::string& Request::type()const{ return _type; }
    const std::string& Request::op()const{ return _op; }
    const std::string& Request::id()const{ return _id; }


    Mappings::ptr Indexer::mappings(){
        Mappings::ptr mp;
        mp=this->getElement<Mappings>("mapping");
        if(mp){
            return mp;
        }
        mp = std::make_shared<Mappings>();
        this->addElement(mp);
        return mp;
    }
    Field::ptr Indexer::field(const std::string& name){
        return this->mappings()->properties()->field(name);
    }


    Indexer::Indexer(const std::string& index):Object(""),Request(index, "_doc", "index", index){};
    Settings::ptr Indexer::settings(){
        auto st = this->getElement<Settings>("settings");
        if(st){
            return st;
        }
        st = std::make_shared<Settings>();
        this->addElement(st);
        return st;
    }


    Tokenizer::ptr Indexer::tokenizer(const std::string& name){
        return this->settings()->analysis()->analyzer()->tokenizer(name);
    }


    Inserter::Inserter(const std::string& index, const std::string& id)
        :Object(""),Request(index, "_doc", "_insert", id){}

    Updater::Updater(const std::string& index, const std::string& id)
        :Object(""),Request(index, "_doc", "_update", id){}
    Object::ptr Updater::doc(){
        Object::ptr dc;
        dc=this->getElement<Object>("doc");
        if(dc){
            return dc;
        }
        dc = std::make_shared<Object>("doc");
        this->addElement(dc);
        return dc;
    }

    Deleter::Deleter(const std::string& index, const std::string& id)
        :Object(""),Request(index, "_doc", "_delete", id){}

    Searcher::Searcher(const std::string& index)
        :Object(""),Request(index, "_doc", "_search", ""){}

    Query::ptr Searcher::query(){
        Query::ptr q = this->getElement<Query>("query");
        if(q){
            return q;
        }
        q = std::make_shared<Query>();
        this->addElement(q);
        return q;
    }

    void Searcher::size(size_t sz){
        this->add("size", sz);
    }
    void Searcher::from(size_t fr){
        this->add("from", fr);
    }

    ESClient::ESClient(const std::vector<std::string>& hosts)
        : _client(std::make_shared<elasticlient::Client>(hosts)){}
    
    bool ESClient::create(const Indexer& idx){
        //获取关键信息
        std::string index_name=idx.index();
        std::string index_type=idx.type();
        std::string index_id=idx.id();
        std::string index_body=idx.to_string();
        //
        auto resp=_client->index(index_name, index_type, index_id, index_body);
        if(resp.status_code<200||resp.status_code>=300){
            ERR("创建索引失败: {}/{}--{}",index_name, index_type,resp.text);
            return false;
        }
        return true;
    }   
    bool ESClient::insert(const Inserter& ins){
        std::string index_name=ins.index();
        std::string index_type=ins.type();
        std::string doc_id=ins.id();
        std::string index_body=ins.to_string();
        //
        auto resp=_client->index(index_name, index_type, doc_id, index_body);
        if(resp.status_code<200||resp.status_code>=300){
            ERR("新增数据失败: {}/{}--{}",index_name, index_type,resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::update(const Updater& upd){
        std::string index_name=upd.index();
        std::string index_type=upd.type();
        std::string doc_id=upd.id();
        std::string index_body=upd.to_string();
        std::string url=index_name+"/_update/"+doc_id;
        //
        auto resp=_client->performRequest(elasticlient::Client::HTTPMethod::POST, url, index_body);
        if(resp.status_code<200||resp.status_code>=300){
            ERR("更新数据失败: {}/{}--{}",index_name, index_type,resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::remove(const Deleter& del){
        std::string index_name=del.index();
        std::string index_type=del.type();
        std::string doc_id=del.id();
        //
        auto resp=_client->remove(index_name, index_type, doc_id);
        if(resp.status_code<200||resp.status_code>=300){
            ERR("删除数据失败: {}/{}--{}",index_name, index_type,resp.text);
            return false;
        }
        return true;
    }
    bool ESClient::remove(const std::string& index){
        std::string index_name=index;
        auto resp=_client->performRequest(elasticlient::Client::HTTPMethod::DELETE, index_name, "");
        if(resp.status_code<200||resp.status_code>=300){
            ERR("删除索引失败: {}--{}",index_name,resp.text);  
            return false;
        }    
        return true;
    }
    std::optional<Json::Value> ESClient::search(const Searcher& sea){
        std::string index_name=sea.index();
        std::string index_type=sea.type();
        std::string index_body=sea.to_string();
        //
        auto resp=_client->search(index_name, index_type, index_body);
        if(resp.status_code<200||resp.status_code>=300){
            ERR("搜索数据失败: {}/{}--{}",index_name, index_type,resp.text);
            return std::optional<Json::Value>();
        }
        auto json_resp=linutil::JSON::unserialize(resp.text);
        if(!json_resp){
            ERR("搜索数据失败，返回结果非JSON格式: {}/{}--{}",index_name, index_type,resp.text);
            return std::optional<Json::Value>();
        }
        if((*json_resp).isNull()||(*json_resp)["hits"].isNull()||(*json_resp)["hits"]["hits"].isNull()){
            ERR("搜索数据失败，返回结果为空: {}/{}--{}",index_name, index_type,resp.text);
            return std::optional<Json::Value>();
        }
        Json::Value result;
        int sz=(*json_resp)["hits"]["hits"].size();
        for(int i=0;i<sz;i++){
            result.append((*json_resp)["hits"]["hits"][i]["_source"]);
        }
        return result;
    }
}