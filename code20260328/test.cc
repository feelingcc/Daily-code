#include "person.pb.h"
#include <iostream>

std::string serialize_test() {
    person::Student stu;
    stu.set_id(1001);
    stu.set_name("zhangsan");
    auto phone1 = stu.add_phones();
    phone1->set_number("123456789");
    phone1->set_type(person::PhoneType::HOME);
    
    auto scores = stu.mutable_scores();
    (*scores)["math"] = 95;
    (*scores)["english"] = 85.5;
    scores->insert({"chinese", 100});

    std::string str = stu.SerializeAsString();
    std::cout << str << std::endl;
    return str;
}

void unserialize_test(const std::string& str) {
    person::Student stu;
    bool ret = stu.ParseFromString(str);
    if (ret == false) {
        std::cout << "unserialize failed" << std::endl;
        return;
    }
    std::cout << stu.id() << std::endl;
    std::cout << stu.name() << std::endl;
    for (int i = 0; i < stu.phones().size(); ++i) {
        std::cout << stu.phones(i).number() << " " << stu.phones(i).type() << std::endl;
    }
    auto it = stu.scores().find("math");
    if (it != stu.scores().end()) {
        std::cout << it->second << std::endl; // 数学成绩
    }
    for (auto it: stu.scores()) {
        std::cout << it.first << " " << it.second << std::endl;
    }
}

int main() {

    std::cout << "序列化结果：" << std::endl;
    std::string res = serialize_test();
    std::cout << res << std::endl;
    std::cout << "反序列化结果：" << std::endl;
    unserialize_test(res);

    return 0;
}