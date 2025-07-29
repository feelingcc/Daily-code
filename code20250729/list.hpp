#pragma once

#include <iostream>
#include <cassert>

namespace simulate_list {
    template<typename T>
    struct list_node{
        T val;
        list_node* prev;
        list_node* next;
        list_node(const T& data = T()) :val(data) , prev(nullptr) , next(nullptr) {}
    };

    template<typename T , typename Ref , typename Ptr>
    struct __list_iterator {
        typedef list_node<T> node;
        typedef __list_iterator<T , Ref , Ptr> self;

        __list_iterator(node* nodeptr) :cur(nodeptr) {}

        self& operator++() {
            cur = cur->next;
            return *this;
        }

        self operator++(int) {
            self temp(cur);
            cur = cur->next;
            return temp;
        }

        self& operator--() {
            cur = cur->prev;
            return *this;
        }

        self operator--(int) {
            self temp(cur);
            cur = cur->prev;
            return temp;
        }

        Ref operator*() {
            return cur->val;
        }

        Ptr operator->() {
            return &(operator*());
        }

        bool operator==(const self& s) {
            return cur == s.cur;
        }

        bool operator!=(const self& s) {
            return cur != s.cur;
        }

        node* cur;
    };

    template<typename T>
    class list {
            typedef list_node<T> node;
        public:
            typedef __list_iterator<T , T& , T*> iterator;
            typedef __list_iterator<T , const T& , const T*> const_iterator;

            iterator begin() { return iterator(head->next); }
            iterator end() { return iterator(head); }
            const_iterator begin() const { return const_iterator(head->next); }
            const_iterator end() const { return const_iterator(head); }
            bool empty() const { return head->next == head; }
            T& front() { assert(!empty()); return head->next->val; }
            T& back() { assert(!empty()); return head->prev->val; }
            const T& front() const { assert(!empty()); return head->next->val; }
            const T& back() const { assert(!empty()); return head->prev->val; }
            size_t size() const {
                size_t count = 0;
                for (auto& a : *this) count++;
                return count; 
            }
            void clear() {
                auto it = begin();
                while (it != end())
                    it = erase(it);
            }

            void initializer_list() {
                head = new node;
                head->prev = head;
                head->next = head;
            }
            list<T>() {
                initializer_list();
            }
            list<T>(const list<T>& l) {
                initializer_list();
                for (auto& node : l)
                    push_back(node);
            }
            list<T>& operator=(list<T> l) {
                std::swap(head , l.head);
                return *this;
            }
            ~list<T>() {
                clear();
                delete head;
                head = nullptr;
            }

            iterator insert(iterator position , const T& val);
            iterator erase(iterator position);
            void push_front(const T& val) { insert(begin() , val); }
            void pop_front() { assert(!empty()); erase(begin()); }
            void push_back(const T& val) { insert(end() , val); }
            void pop_back() { assert(!empty()); erase(--end()); }  

        private:
            node* head = nullptr;
    }; 
    
    template<typename T>
    typename list<T>::iterator list<T>::insert(list<T>::iterator position , const T& val) {
        node* prev = position.cur->prev;
        node* newnode = new node(val);
        prev->next = newnode;
        newnode->prev = prev;
        newnode->next = position.cur;
        position.cur->prev = newnode;
        return iterator(newnode);   // 返回新插入节点的迭代器
    }

    template<typename T>
    typename list<T>::iterator list<T>::erase(list<T>::iterator position) {
        node* prev = position.cur->prev;
        node* next = position.cur->next;
        prev->next = next;
        next->prev = prev;
        delete position.cur;
        return iterator(next);  // 返回删除元素的下一个元素
    }
    
} // namespace simulate_list end