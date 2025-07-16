
#include<iostream>
using namespace std;

class Test {
public:
    Test(int v) :variable(v) { cout << "Test(int v)" << endl; }
    Test(const Test& test) :variable(test.variable) { cout << "Test(const Test& test)" << endl; }
    Test& operator=(const Test& test) {
        if (this != &test)
        {
            variable = test.variable;
        }
        cout << "Test& operator=(const Test& test)" << endl;
        return*this;
    }
    ~Test() { cout << "~Test()" << endl; }
private:
    int variable;
};

Test func1() {

    Test test(1);
    return test;
}

void func2(Test test) {
    
}

int main() {

    // 隐式类型转换 1.常量1构造一个临时的Test对象 2.临时的对象拷贝构造test1对象
    // 优化：构造 + 拷贝构造 -> 直接构造
    Test test1(1);
    // 1.构造匿名对象 2.匿名对象拷贝构造test2对象
    // 优化：构造 + 拷贝构造 -> 直接构造
    Test test2(Test(1));

    // 1.构造局部变量test对象 2.构造临时对象 3.局部test对象拷贝构造临时对象 4.临时对象拷贝构造test3对象
    // 优化：1.直接使用1构造临时对象 2.临时对象拷贝构造test3对象
    Test test3 = func1();

    // 1.常量1构造一个临时的Test对象 2.临时的对象拷贝构造函数形参test对象
    // 优化：构造 + 拷贝构造 -> 直接构造
    func2(Test(1));

    // 为什么中间会有临时对象，可以通过引用验证，普通引用会报错，而const引用可以。
    // 说明中间会产生临时对象，而临时对象具有常性。
    // Test& ref = 1;  error
    const Test& ref = 1;

    return 0;
}


//#include<iostream>
//using namespace std;
//class A
//{
//	public :
//	A(int a = 0)
//		: _a1(a)
//	{
//		cout << "A(int a)" << endl;
//	} 
//	A(const A& aa)
//		:_a1(aa._a1)
//	{
//		cout << "A(const A& aa)" << endl;
//	} 
//	A& operator=(const A& aa)
//	{
//		cout << "A& operator=(const A& aa)" << endl;
//		if (this != &aa)
//		{
//			_a1 = aa._a1;
//		} 
//		return* this;
//	} 
//	~A()
//	{
//		cout << "~A()" << endl;
//	}
//private: 
//	int _a1 = 1;
//};
//void f1(A aa)
//{}
//A f2()
//{
//	A aa;
//	return aa;
//} 
//
//int main()
//{
//
//	A aa = 1;
//
//	// 传值传参
//	// 构造+拷⻉构造
//	//A aa1;
//	//f1(aa1);
//	//cout << endl;
//}

//#include <iostream>
//#include <fstream>
//#include <string>
//using namespace std;
//
//class Obj {
//public:
//	//Obj() = default;
//
//	Obj() {}
//
//	Obj(int _a) :a(_a){
//		this->a;
//	}
//
//	int a = 11;
//};
//
//int main() {
//
//	Obj obj;
//
//	return 0;
//}

//void fun(int a) {
//	printf("xxx");
//}
//
//int main() {
//	fun();
//}

//typedef int STDataType;
//class Stack
//{
//public:
//	Stack(int n = 4)
//	{
//		_a = (STDataType*)malloc(sizeof(STDataType) * n);
//		if (nullptr == _a)
//		{
//			perror("malloc申请空间失败");
//			return;
//		}
//		_capacity = n;
//		_top = 0;
//	}
//	STDataType* _a;
//	size_t _capacity;
//	size_t _top;
//};
//// 两个Stack实现队列
//class MyQueue
//{
//public:
//	//编译器默认⽣成MyQueue的构造函数调⽤了Stack的构造，完成了两个成员的初始化
//	MyQueue() {}
//private:
//	Stack pushst;
//	Stack popst;
//};
//int main()
//{
//	MyQueue mq;
//	return 0;
//}


//int test() {
//    int x = 100;
//    return x;
//}
//
//int main() {
//
//    const int& ref = test();
//    cout << ref << endl;
//
//    return 0;
//}

//namespace longname {
//	int x = 100;
//}
//
//namespace newName = longname;
//
//int main() {
//
//	cout << newName::x <<" " << longname::x << endl;
//
//}

//bool isPalindrome(string s) {
//	int left = 0;
//	int right = s.size() - 1;
//	while (left < right) {
//		if (s[left] == s[right]) {
//			left++;
//			right--;
//		}
//		else {
//			return false;
//		}
//	}
//	return true;
//}

//bool isPalindrome(string s) {
//	string t = s;
//	reverse(s.begin(), s.end());
//	return t == s;
//}
//
//int main() {
//
//	for (int i = 2; i < 10000; i++) {
//		bool flag = true;
//		for (int j = 2; j < i; j++) {
//			if (i % j == 0) {
//				flag = false;
//				break;
//			}
//		}
//		if (flag) {
//			// 当前 i 是素数
//			// 判断是否是回文的
//			// 简单方法：将当前数转换为字符串判断
//			if (isPalindrome(to_string(i)))
//				cout << i << " ";
//		}
//	}
//
//	return 0;
//}

//int main() {
//
//	ifstream afile("D:\\a.txt");
//	ifstream bfile("D:\\b.txt");
//	if (!afile) {
//		std::cout << "a文件打开失败" << std::endl;
//		return 1;
//	}
//	if (!bfile) {
//		std::cout << "b文件打开失败" << std::endl;
//		return 1;
//	}
//	string res;
//	char c1, c2;
//	// eof 达到文件末尾返回true
//	while (!afile.eof() || !bfile.eof()) {
//		if (!afile.eof()) {
//			afile.get(c1);
//			res += c1;
//		}
//		if (!bfile.eof()) {
//			bfile.get(c2);
//			res += c2;
//		}
//	}
//	cout << res; // debug
//
//	afile.close();
//	bfile.close();
//	ofstream outafile("D:\\a.txt");
//	if (!outafile) {
//		std::cout << "a文件打开失败" << std::endl;
//		return 1;
//	}
//	outafile << res;
//	outafile.close();
//
//	return 0;
//}

//class Stu {
//public:
//	Stu() {}
//
//	Stu(int _id, const string& _name , double _score) 
//		:id(_id) , name(_name) , score(_score)
//	{}
//
//	int id;
//	string name;
//	double score;
//};
//
//void input(Stu& stu) {
//	cin >> stu.id >> stu.name >> stu.score;
//}
//
//void print(Stu& stu) {
//	cout << stu.id << " "  << stu.name << " " << stu.score;
//}
//
//int main() {
//
//	Stu stu;
//	input(stu);
//	print(stu);
//
//	return 0;
//}

//const double pi = 3.1415926;
//
//class Shape {
//public:
//	virtual double circumference() = 0;
//	virtual double area() = 0;
//};
//
//class Circle : public Shape {
//public:
//	Circle(double _r) :r(_r) {
//		
//	}
//	virtual double circumference() {
//		return 2 * pi * r;
//	}
//	virtual double area() {
//		return pi * r * r;
//	}
//protected:
//	double r;
//};
//
//// 梯形的周长 = 上底边长度 + 下底边长度 + 两个腰边的长度之和
//// 梯形的面积 = （上底 + 下底） * 高 / 2
//// 直角三角形的面积 = 底 * 高 / 2;
//
//
//int main() {
//
//
//
//	return 0;
//}

//class Point {
//public:
//	void initP(float xx = 0, float yy = 0) {
//		x = xx;
//		y = yy;
//	}
//	void move(float xoff, float yoff) {
//		x += xoff;
//		y += yoff;
//	}
//	float getx() { return x; }
//	float gety() { return y; }
//private:
//	float x, y;
//};
//
//class Rectangle : public Point {
//public:
//	void initR(float x, float y, float ww, float hh) {
//		initP(x , y);
//		w = ww;
//		h = hh;
//	}
//	float geth() { return h; }
//	float getw() { return w; }
//private:
//	float w, h;
//};
//
//int main() {
//
//	Rectangle rect;
//	rect.initR(2 , 3 , 20 , 10);
//	rect.move(3 , 2);
//	cout << rect.getx() << " " << rect.gety() << " " << rect.getw() << " " << rect.geth() << " ";
//
//	return 0;
//}

//#include "priority_queue.h"
//using namespace simulate;
//
//int main() {
//
//	priority_queue<int> heap;
//	heap.push(2);
//	heap.push(4);
//
//	while (!heap.empty()) {
//		std::cout << heap.top() << "";
//		heap.pop();
//	}
//	std::cout << std::endl;
//
//	return 0;
//}