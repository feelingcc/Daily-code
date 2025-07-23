#include <iostream>
#include <string>
using namespace std;

int main() {

    string str("hello world"  , 5);
    cout << str << endl;

    //std::string s = "hello world";
    //s.resize(20);
    //s += "xxx";
    //std::cout << s << std::endl;
    //std::cout << s.c_str() << std::endl;

	return 0;
}

//class A {
//public:
//	~A() {
//		cout << "aaa" << endl;
//	}
//	int a;
//};
//
//class B {
//public:
//	
//	int b;
//};
//
//
//int main() {
//
//	//A* ptr = new A[3];
//	//delete ptr;
//
//	B* ptr = new B[3];
//	delete ptr;
//
//	return 0;
//}