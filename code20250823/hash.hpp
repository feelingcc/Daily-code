#pragma once 
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

namespace hash_table {
    inline unsigned long __stl_next_prime(unsigned long n)  {
        // Note: assumes long is at least 32 bits.
        static const int __stl_num_primes = 28;
        static const unsigned long __stl_prime_list[__stl_num_primes] = {
            53, 97, 193, 389, 769,
            1543, 3079, 6151, 12289, 24593,
            49157, 98317, 196613, 393241, 786433,
            1572869, 3145739, 6291469, 12582917, 25165843,
            50331653, 100663319, 201326611, 402653189, 805306457,
            1610612741, 3221225473, 4294967291
        };
        const unsigned long* first = __stl_prime_list;
        const unsigned long* last = __stl_prime_list + __stl_num_primes;
        const unsigned long* pos = std::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }

    template<typename Key , typename Value>
    struct HashTableNode {
        std::pair<Key , Value> data;
        HashTableNode<Key , Value>* next;
        HashTableNode(const std::pair<Key , Value>& _data) :data(_data) , next(nullptr) {}
    };

    template<typename Key , typename Value>
    class HashTable {
        using HashNode = HashTableNode<Key , Value>;
        public:
            HashTable()
                :table(__stl_next_prime(0)) , n(0)
            {}

            bool insert (std::pair<Key , Value>& data) {
                // 负载因子为1时，hash需要扩容
                if (n == table.size()) {
                    std::vector<HashNode*> newTable(__stl_next_prime(table.size() + 1));
                    // 将旧表中的节点依次转移到新表
                    for(auto head : table) {
                        HashNode* cur = head;
                        while (cur) {
                            HashNode* next = cur->next;
                            size_t key = cur->data.first % newTable.size();
                            cur->next = newTable[key];
                            newTable[key] = cur;
                            cur = next;
                        }
                        head = nullptr;
                    }
                    // 交换旧表和新表
                    table.swap(newTable);
                }
                // 添加新节点
                HashNode* newnode = new HashNode(data);
                size_t key = data.first % table.size();
                newnode->next = table[key];
                table[key] = newnode;
                n++;

                return true;
            }
        private:
            std::vector<HashNode*> table;
            size_t n;
    };

} // end namespace hash_table