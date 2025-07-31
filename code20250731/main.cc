#include "stack-queue-priority_queue.hpp"
#include <iostream>

void test1 () {
    simulate_container_adapters::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    while (!st.empty()) {
        std::cout << st.top() << std::endl;
        st.pop();
    }
}

void test2 () {
    simulate_container_adapters::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    while (!q.empty()) {
       std::cout << q.front() << std::endl;
       q.pop();
    }
}

void test3 () {
    // simulate_container_adapters::priority_queue<int> max_pq;
    simulate_container_adapters::priority_queue<int ,std::vector<int> , std::greater<int>> max_pq;
    max_pq.push(4);
	max_pq.push(1);
	max_pq.push(5);
	max_pq.push(7);
	max_pq.push(9);
	while (!max_pq.empty())
	{
		std::cout << max_pq.top() << " ";
		max_pq.pop();
	}
	std::cout << std::endl;
}

void test () {
    // test1();
    // test2();
    test3();
}

int main () {

    test();

    return 0;
}