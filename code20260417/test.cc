#include "elasticsearch.hpp"

void test1() {
    es::Base root("ikmax");
    root.add("tokenizer" , "ik_max_word");
    root.add("type" , "custom");
    std::cout << root.to_json_string() << std::endl;
}

void test2() {
    std::shared_ptr<es::Base> ik_max = std::make_shared<es::Base>("ikmax");
    ik_max->add("tokenizer" , "ik_max_word");
    ik_max->add("type" , "custom");
    es::Object obj("analyzer");
    obj.addElement(ik_max);
    std::cout << obj.to_json_string() << std::endl;
}

void test3() {
    es::Analyzer analyzer;
    analyzer.tokenizer("standered");
    std::cout << analyzer.to_json_string() << std::endl;
}

int main() {

    // test2();
    test3();

    return 0;
}