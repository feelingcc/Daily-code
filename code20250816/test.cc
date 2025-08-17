#include "AVL.hpp"
#include <vector>

void TestAVLTree1()
{
	AVL::AVLTree<int, int> t;
	// 常规的测试用例
	// int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	// 特殊的带有双旋场景的测试用例
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };

	for (auto e : a)
	{
		t.insert({ e, e });
	}

	t.InOrder();
	std::cout << t.IsBalanceTree() << std::endl;
}

// 插入一堆随机值，测试平衡，顺便测试一下高度和性能等
void TestAVLTree2()
{
	const int N = 1000000;
	std::vector<int> v;
	v.reserve(N);
	srand(time(0));
	for (size_t i = 0; i < N; i++)
	{
		v.push_back(rand() + i);
	}

	size_t begin2 = clock();
	AVL::AVLTree<int, int> t;
	for (auto e : v)
	{
		t.insert(std::make_pair(e, e));
	}
	size_t end2 = clock();

	std::cout << "Insert:" << (double)(end2 - begin2) / CLOCKS_PER_SEC  << std::endl;
	std::cout << t.IsBalanceTree() << std::endl;

	std::cout << "Height:" << t.Height() << std::endl;
	std::cout << "Size:" << t.Size() << std::endl;

	size_t begin1 = clock();
	// 确定在的值
	for (auto e : v)
	{
		t.Find(e);
	}
	// 随机值
	for (size_t i = 0; i < N; i++)
	{
		t.Find((rand() + i));
	}
	size_t end1 = clock();
	std::cout << "Find:" << (double)(end2 - begin2) / CLOCKS_PER_SEC << std::endl;
}

void test() {
    // TestAVLTree1();
    TestAVLTree2();
}

int main () {

    test();

    return 0;
}