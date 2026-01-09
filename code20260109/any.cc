#include <iostream>
#include <typeinfo>
#include <string>
#include <cassert>
#include <unistd.h>

// 可以赋值任意类型的类
class Any{
    public:
        Any():_ph(nullptr) {}
        template<typename T> Any(const T& val) :_ph(new holder<T>(val)) {}
        Any(const Any& other) :_ph(other._ph ? other._ph->Clone() : nullptr) {} // C++中拷贝构造函数不能设计为模版函数，因此一定需要clone函数
        void swap(Any& other) { std::swap(_ph , other._ph); }
        template<typename T> Any& operator=(const T& val) {
            Any(val).swap(*this);
            return *this;
        }
        Any& operator=(Any other) {
            swap(other);
            return *this;
        }
        template<typename T> T* get() {
            // 获取的类型必须和保存的类型数据一致
            assert(_ph);
            assert(typeid(T) == _ph->Type());
            return dynamic_cast<holder<T>*>(_ph)->GetVal(); // dynamic_cast 在模板场景下要求完全匹配，不只是父类指向子类就行。
        }
        ~Any() { if(_ph) delete _ph; }
    private:
        // 定一个基类
        class placeholder{
            public:
                virtual ~placeholder() {}
                virtual placeholder* Clone() = 0;
                virtual const std::type_info& Type() = 0;
        };
        template<typename T>
        class holder : public placeholder{
            public:
                holder(const T& val) :_val(val) {}
                virtual const std::type_info& Type() override {
                    return typeid(_val);
                }
                virtual placeholder* Clone() override {
                    return new holder<T>(_val);
                }
                T* GetVal() { return &_val; }
            private:
                T _val;
        };
        placeholder* _ph;
};

class Test{
    public:
        Test() {std::cout << "构造" << std::endl;}
        Test(const Test &t) {std::cout << "拷贝" << std::endl;}
        ~Test() {std::cout << "析构" << std::endl;}
};

int main() {

    Any a;
    a.get<int>();

    // Any a;
    // {
    //     Test t;
    //     a = t;
    // }
    
    // a = 10;
    // int *pa = a.get<int>();
    // std::cout << *pa << std::endl;
    // a = std::string("nihao");
    // std::string *ps = a.get<std::string>();
    // std::cout << *ps << std::endl;
    // while(1) sleep(1);

    // Any any(10);
    // int* ptr = any.get<int>();
    // std::cout << *ptr << std::endl;
    // any = std::string("hello");
    // std::cout << *any.get<std::string>() << std::endl;

    return 0;
}