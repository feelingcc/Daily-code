#include "hash.hpp"

void print(hash_bucket::HashTable<int, int> &h)
{
    auto it = h.begin();
    while (it != h.end())
    {
        std::cout << it->first << " " << it->second << std::endl;
        ++it;
    }
    std::cout << "---------------------------------" << std::endl;
}

void test1()
{
    hash_bucket::HashTable<int, int> h;
    int a[] = {3, 1, 6, 7, 888, 288, 188, 1, 5, 6, 7, 6};
    for (auto e : a)
        h.insert({e, e});

    print(h);

    h.erase(1);
    print(h);
    h.erase(3);
    print(h);
    h.erase(5);
    print(h);
    h.erase(6);
    print(h);
    h.erase(7);
    print(h);
    h.erase(288);
    print(h);
    h.erase(188);
    print(h);
    h.erase(88);
    print(h);
}

void test2 () {
    hash_bucket::HashTable<int, int> h;
    int a[] = {3, 1, 6, 7, 888, 288, 188, 1, 5, 6, 7};
    for (auto e : a)
        h.insert({e, e});
    for (int i = 0; i < 1000; i++) {
        h.insert({ i ,  i});
    }
    hash_bucket::HashTable<int, int> hh(h);
    print(h);
    hh.erase(1);
    print(hh);
}

int main()
{

    // test1();
    test2();

    return 0;
}