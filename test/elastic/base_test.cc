#include "../../source/elastic.h"
#include <iostream>

int main(){
    // linelastic::Indexer indexer;
    // // indexer.settings()->analysis()->analyzer()->tokenizer("ikmax");
    // // std::string str = indexer.to_string();

    // // linelastic::Indexer indexer;
    // // auto tk=indexer.tokenizer("ikmax");

    // // //auto tokenizer=indexer.tokenizer("standard");
    // // // tokenizer->tokenizer("standard");
    // // // tokenizer->type("custom");
    // // tk->add("key1","value1");
    // indexer.tokenizer("ikmax");

    // auto name = indexer.field("name");
    // name->analyzer("ikmax");

    // auto phone = indexer.field("phone");
    // phone->type("keyword"); 
 
    // std::string str = indexer.to_string();      
    // std::cout<<str<<std::endl;  

    // linelastic::Searcher searcher;
    // auto query = searcher.query();

    // auto terms = query->terms("status");
    // terms->setValue(1);
    // terms->setValue(2);
    // terms->setValue(3);
    
    // auto multi_match = query->multi_match();
    // // auto field=multi_match->newObject("field");    
    // multi_match->appendField("title");
    // multi_match->appendField("description");   
    // multi_match->setQuery("quick fox");

    // auto range = query->range("age");
    // range->setRange(10, 20);


    // auto term = query->term("user");
    // term->setValue("kimchy");

    // auto match = query->match("message");
    // match->setValue("this is a test");

    // query->match_all();


    // linelastic::Qarray arr("must");
    // arr.term("user")->setValue("kimchy");
    // arr.match("message")->setValue("this is a test");

    // auto multi_match = arr.multi_match();
    // multi_match->appendField("title");  
    // multi_match->appendField("description");  
    // multi_match->setQuery("quick fox");

    // linelastic::Searcher searcher;
    // auto must= searcher.query()->must();
    // must->term("user")->setValue("kimchy");
    // must->match("message")->setValue("this is a test");
    // auto should= searcher.query()->should();
    // auto terms= should->terms("status");
    // terms->setValue(1);
    // terms->setValue(2);
    // terms->setValue(3);
    // auto must_not= searcher.query()->must_not();
    // must_not->range("age")->setRange(10,20);
    // searcher.query()->qbool()->minimum_should_match(1);


    // std::cout << searcher.to_string() << std::endl;
    linelastic::Indexer indexer("student");
    indexer.tokenizer("ik_smart");
    auto name = indexer.field("name");
    name->analyzer("ik_smart");
    indexer.field("age")->type("integer");
    indexer.field("score")->type("float");
    
    linelastic::Inserter inserter("student", "1");
    inserter.add("name", "张三");
    inserter.add("age", 20);
    inserter.add("score", 88.5);
    std::cout << inserter.index()<<'/'<< inserter.id() << std::endl;
    std::cout << inserter.to_string() << std::endl;

    linelastic::Updater updater("student", "1");
    updater.doc()->add("score", 92.0);
    std::cout << updater.index()<<'/'<< updater.id() << std::endl;
    std::cout << updater.to_string() << std::endl;

    linelastic::Searcher searcher("student");
    auto query = searcher.query();
    query->must()->match("name")->setValue("alice");
    query->must()->range("age")->setRange(18, 25);
    query->must()->terms("score")->setValue(90);

    auto mm=query->should()->multi_match();
    mm->appendField("name");
    mm->appendField("score");
    mm->setQuery("alice 90");

    searcher.size(5);
    searcher.from(0);
    std::cout << searcher.to_string() << std::endl;
    return 0;
}
