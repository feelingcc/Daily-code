#include "list.hpp"

#include <list>

void test1()
{
    // 测试 std::list 是否会哨兵卫头节点
    std::list<int> l;
    l.push_back(1);
    std::list<int>::iterator it = l.erase(l.begin());
    std::cout << *it << std::endl;
}

void test2()
{
    simulate_list::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_front(3);
    l.push_front(4);
    for (auto a : l)
        std::cout << a << " ";
    std::cout << std::endl;
    simulate_list::list<int> temp = l;
    for (auto a : temp)
        std::cout << a << " ";
    temp.pop_back();
    temp.pop_front();
    std::cout << std::endl;
    for (auto a : temp)
        std::cout << a << " ";
    std::cout << std::endl;
    std::cout << l.size() << std::endl;
}

void test()
{
    // test1();
    test2();
}

int main()
{

    test();

    return 0;
}