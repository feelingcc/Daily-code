// struct LNode{
//     int key , val;
//     LNode* next;
//     LNode* prev;
//     LNode() : key(-1) , val(-1) , next(nullptr) , prev(nullptr) {}
//     LNode(int k , int v) : key(k) , val(v) , next(nullptr) , prev(nullptr) {}
// };

// class LRUCache {
// private:
//     // 双向链表即可 不用循环双向，还得多维护指针
//     LNode* head; // 头部是最近最少使用，每次使用后转移到尾部
//     LNode* tail;
//     std::unordered_map<int , LNode*> hash;
//     int _capacity;
//     int _size;
// public:
//     LRUCache(int capacity) :_capacity(capacity) , _size(0)  {
//         head = new LNode(-1 , -1);
//         tail = head;
//     }
    
//     int get(int key) {
//         if(hash.count(key)) { // 存在
//             // 更新缓存
//             LNode* node = hash[key];
//             // 考虑最后一个节点就不会越界问题了
//             if(node == tail) {
//                 // 已经是最后一个节点了
//                 return node->val;
//             }
//             node->prev->next = node->next;
//             node->next->prev = node->prev;
//             node->prev = tail;node->next = nullptr;
//             tail->next = node;
//             tail = node;
//             return tail->val;
//         }
//         return -1;
//     }
    
//     void put(int key, int value) {
//         if(hash.count(key)) { // 存在
//             LNode* node = hash[key];
//             if(node == tail) {
//                 node->val = value;
//                 return;
//             }
//             node->prev->next = node->next;
//             node->next->prev = node->prev;
//             node->prev = tail;node->next = nullptr;
//             tail->next = node;
//             tail = node;
//             tail->key = key; tail->val = value;
//             return;
//         }
//         // 不存在
//         LNode* node = new LNode(key , value);
//         hash[key] = node;
//         node->prev = tail;tail->next = node;
//         tail = node;
//         _size++;
//         // 添加完检查容量
//         if(_size > _capacity) {
//             // 移除最前面的
//             LNode* first = head->next;
//             head->next = first->next; 
//             first->next->prev = head;
//             hash.erase(first->key);
//             if(tail == first) tail = head;
//             delete first;
//             _size--;
//         }
//     }
// };

// /**
//  * Your LRUCache object will be instantiated and called as such:
//  * LRUCache* obj = new LRUCache(capacity);
//  * int param_1 = obj->get(key);
//  * obj->put(key,value);
//  */

//  class Solution {
// public:
//     vector<int> spiralOrder(vector<vector<int>>& matrix) {
//         int m = matrix.size() , n = matrix[0].size();
//         vector<int> ans;
//         // 定义上下左右四个边界
//         int top = 0 , bottom = m - 1 , left = 0 , right = n - 1;
//         // 循环结束条件，因为top是向下收缩，bottom是向上收缩，left、right同理，而当left=right且top=bottom的时候，这个元素也需要添加，因此是 <= 
//         while(top <= bottom && left <= right) {
//             for(int j = left ; j <= right; j++) ans.push_back(matrix[top][j]);
//             top++;
//             if(top > bottom) break;
//             for(int i = top; i <= bottom; i++) ans.push_back(matrix[i][right]);
//             right--;
//             if(right < left) break;
//             for(int j = right; j >= left; j--) ans.push_back(matrix[bottom][j]);
//             bottom--;
//             if(bottom < top) break;
//             for(int i = bottom; i >= top; i--) ans.push_back(matrix[i][left]);
//             left++;
//         }
//         return ans;
//     }
// };