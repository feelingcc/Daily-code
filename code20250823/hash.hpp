#pragma once 
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

namespace hash_bucket {
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

    template<typename Key>
    struct HashK {
        size_t operator()(const Key& key) { return (size_t)key; }
    };

    template<>
    struct HashK<std::string> {
        size_t operator()(const std::string& key) {
            size_t res = 0;
            for (auto c : key) {
                res += c * 131;
            }
            return res;
        }
    };

    template<typename Key , typename Value>
    struct HashTableNode {
        std::pair<Key , Value> data;
        HashTableNode<Key , Value>* next;
        HashTableNode(const std::pair<Key , Value>& _data) :data(_data) , next(nullptr) {}
    };

    template<typename Key>
    struct IsEqualKey {
        bool operator()(const Key& k1 , const Key& k2) {
            return k1 == k2;
        }
    };

    //  前置声明
    //  默认参数只能在声明时写，不能在定义时写
    template<typename Key , typename Value , typename HashKey = HashK<Key> , typename EqualKey = IsEqualKey<Key>>
    class HashTable;

    template<typename Key , typename Value , typename Ref , typename Ptr , typename HashKey = HashK<Key> , typename EqualKey = IsEqualKey<Key>>
    struct HashIterator {
        using HashNode = HashTableNode<Key , Value>;
        using Ht = HashTable<Key , Value , HashKey , EqualKey>;
        using Self = HashIterator<Key , Value , Ref , Ptr , HashKey , EqualKey>;
        HashNode* cur;
        Ht* ht;
        HashIterator(HashNode* node , Ht* _ht) :cur(node) , ht(_ht) {}
        // 前置++
        Self& operator++() {
            if (cur->next) {
                cur = cur->next;
            } else {
                size_t key = HashKey()(cur->data.first) % ht->table.size();
                key++;
                while (key < ht->table.size() && !ht->table[key]) {
                    key++;
                }
                if (key == ht->table.size()) {
                    cur = nullptr;
                } else {
                    cur = ht->table[key];
                }
            }
            return *this;
        }

        Ref operator* () {
            return cur->data;
        }

        Ptr operator-> () {
            return &(cur->data);
        }

        bool operator== (const Self& s) {
            return cur == s.cur;
        }
        
        bool operator!= (const Self& s) {
            return cur != s.cur;
        }
    };

    // HashKey仿函数：将key转换为无符号整数
    // EqualKey仿函数：key需要支持 == 运算符
    template<typename Key , typename Value , typename HashKey , typename EqualKey>
    class HashTable {
        // 内层友元模板，不能和外层HashTable模板参数相同
        template<typename K2 , typename V2 , typename Ref2 , typename Ptr2 , typename HK2 , typename EK2>
        friend struct HashIterator;

        using HashNode = HashTableNode<Key , Value>;
        public:
        typedef HashIterator<Key , Value , std::pair<Key , Value>& , std::pair<Key , Value>* , HashKey , EqualKey> iterator;
            // using iterator = ;
            using const_iterator = HashIterator<Key , Value , const std::pair<Key , Value>& ,  const std::pair<Key , Value>* , HashKey , EqualKey>;

            iterator begin() {
                if (n == 0) {
                    return end();
                }

                for (size_t i = 0; i < table.size(); i++) {
                    if (table[i]) {
                        return iterator(table[i], this);
                    }
                }

                return end();
            }

            iterator end() {
                return iterator(nullptr , this);
            }

            HashTable()
                :table(__stl_next_prime(0)) , n(0)
            {}

            ~HashTable() {
                for (size_t i = 0; i < table.size(); i++) {
                    HashNode* cur = table[i];
                    while (cur) {
                        HashNode* next = cur->next;
                        delete cur;
                        cur = next;
                    }
                    table[i] = nullptr;
                }
            }

            HashTable(const HashTable& ht) {
               table.resize(ht.table.size());
               n = ht.n;
               for (size_t i = 0; i < ht.table.size(); i++) {
                    HashNode* cur = ht.table[i];
                    if (!cur) {
                        table[i] = nullptr;
                        continue;
                    }
                    while (cur) {
                        HashNode* newnode = new HashNode(cur->data);
                        newnode->next = table[i];
                        table[i] = newnode;
                        cur = cur->next;
                    }
                }
            }
            
            HashTable& operator=(HashTable ht) {
                table.swap(ht.table);
                std::swap(n  , ht.n);
                return *this;
            }

            bool insert (const std::pair<Key , Value>& data) {
                if (find(HashKey()(data.first))) return false; // 存在相同元素，不插入

                // 负载因子为1时，hash需要扩容
                if (n == table.size()) {
                    std::vector<HashNode*> newTable(__stl_next_prime(table.size() + 1));
                    // 将旧表中的节点依次转移到新表
                    for(auto head : table) {
                        HashNode* cur = head;
                        while (cur) {
                            HashNode* next = cur->next;
                            size_t key = HashKey()(cur->data.first) % newTable.size();
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
                size_t key = HashKey()(data.first) % table.size();
                newnode->next = table[key];
                table[key] = newnode;
                n++;

                return true;
            }

            bool erase(const Key& key) {
                size_t hashIndex = HashKey()(key) % table.size();
                HashNode* prev = nullptr;
                HashNode* cur = table[hashIndex];
                
                while (cur) {
                    // 删除需要区分头删还是中间节点删除
                    if (EqualKey()(HashKey()(cur->data.first) , HashKey()(key))) {
                        if (!prev) {
                            // 头删
                            table[hashIndex] = cur->next;
                        } else {
                            // 中间节点删除
                            prev->next = cur->next;
                        }
                        delete cur;
                        return true;
                    } else {
                        // 迭代继续寻找
                        prev = cur;
                        cur = cur->next;
                    }
                }
                return false;
            }

            bool find (const Key& key) {
                size_t hashIndex = HashKey()(key) % table.size();
                HashNode* cur = table[hashIndex];
                while (cur) {
                    if (EqualKey()(HashKey()(cur->data.first) , HashKey()(key))) return true;
                    cur = cur->next;
                }
                return false;
            }
            
        private:
            std::vector<HashNode*> table;
            size_t n;
    };

} // end namespace hash_bucket