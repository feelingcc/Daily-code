#include "RBT.hpp"
using namespace std;

void TestRBTree1()
{
	RBT::RBTree<int, int> t;
	// 常规的测试用例
	// int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	// 特殊的带有双旋场景的测试用例
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };

	for (auto e : a)
	{
		t.insert({ e, e });
	}

	t.inorder();
	cout << t.isRBT() << endl;
}

void test () {
    TestRBTree1();
}

int main () {

    test();

    return 0;
}