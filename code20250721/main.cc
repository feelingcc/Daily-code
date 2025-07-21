
#include "string.hpp"
#include <string>

void test1 () {
    std::string s = "hello world";
    s.resize(20);
    s += "xxx";
    std::cout << s << std::endl;
    std::cout << s.c_str() << std::endl;
}

void test2() {
    simulateSTL::string str = "hello world";
    str += 'c';
    str += "hello world";
    str.insert(0 , "abc");
    str.erase(0);
    for (auto c : str) {}
    simulateSTL::string temp(str);
    temp.clear();
    temp = str;
    simulateSTL::string sub = str.substr(10);
}

void test() {
    // test1();
    test2();
}

int main () {

    test();

    return 0;
}