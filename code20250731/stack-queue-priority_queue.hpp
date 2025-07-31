#pragma once

#include <deque>
#include <vector>

namespace simulate_container_adapters {
    template<typename T , typename Container = std::deque<T>>
    class stack {
        public:
            void push(const T& val) { container.push_back(val); }
            void pop() { container.pop_back(); }
            T& top () { return container.back(); }
            size_t size() const { return container.size(); }
            bool empty() const { return container.empty(); }
        private:
            Container container;
    };

    template<typename T , typename Container = std::deque<T>>
    class queue {
        public:
            void push(const T& val) { container.push_back(val); }
            void pop() { container.pop_front(); }
            T& front () { return container.front(); }
            T& back () { return container.back(); }
            size_t size() const { return container.size(); }
            bool empty() const { return container.empty(); }
        private:
            Container container;
    };

    template<typename T>
    struct less {
        bool operator() (const T& x , const T& y) const { return x < y; }
    };

    template<typename T , typename Container = std::vector<T> , typename compare = less<T>>
    class priority_queue{
            void adjust_up (int child) {
                int parent = (child - 1) / 2;
                while (child > 0) {
                    // if (container[parent] < container[child]) {
                    if (compare()(container[parent] , container[child])) {
                        std::swap(container[parent] , container[child]);
                        child = parent;
                        parent = (child - 1) / 2;
                    } else {
                        break;
                    }
                }
            }

            void adjust_down(int parent) {
                int child = parent * 2 + 1;
                while (child < container.size()) {
                    // if (child + 1 < container.size() && container[child] < container[child + 1])
                    if (child + 1 < container.size() && compare()(container[child] , container[child + 1]))
                        child++;
                    // if (container[parent] < container[child]) {
                    if (compare()(container[parent] , container[child])) {
                        std::swap(container[parent] , container[child]);
                        parent = child;
                        child = parent * 2 + 1;
                    } else {
                        break;
                    }
                }
            }

        public:
            void push(const T& val) { 
                container.push_back(val); 
                adjust_up(container.size() - 1);
            }

            void pop() { 
                std::swap(container.front() , container.back());
                container.pop_back(); 
                adjust_down(0);
            }

            T& top () { return container.front(); }
            size_t size() const { return container.size(); }
            bool empty() const { return container.empty(); }
        private:
            Container container;
    };

} // namespace simulate_container_adapters end