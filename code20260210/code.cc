/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
//class Solution {
//    struct compare{
//        bool operator()(ListNode* k1 , ListNode* k2) const {
//            return k1->val > k2->val;
//        }
//    };
//public:
//    ListNode* mergeKLists(vector<ListNode*>& lists) {
//        // 用类似合并两个链表的思路，使用优先级队列对k个指针进行优化
//        priority_queue<ListNode* , vector<ListNode*> , compare> heap;
//        for(auto l : lists) {
//            if(l) heap.push(l);
//        }
//        ListNode* head = new ListNode(-1);
//        ListNode* tail = head;
//        while(!heap.empty()) {
//            ListNode* node = heap.top();
//            heap.pop();
//            tail->next = node;
//            tail = tail->next;
//            if(node->next) heap.push(node->next);
//        }
//        return head->next;
//    }
//};
///**
// * Definition for singly-linked list.
// * struct ListNode {
// *     int val;
// *     ListNode *next;
// *     ListNode() : val(0), next(nullptr) {}
// *     ListNode(int x) : val(x), next(nullptr) {}
// *     ListNode(int x, ListNode *next) : val(x), next(next) {}
// * };
// */
//class Solution {
//public:
//    ListNode* reverseKGroup(ListNode* head, int k) {
//        // 先遍历一遍链表 计算有几组
//        int count = 0;
//        ListNode* cur = head;
//        while(cur) {
//            cur = cur->next;
//            count++;
//        }
//        count /= k;
//        ListNode* prev_group_tail = nullptr;
//        ListNode* group_head = head;
//        for(int i = 0; i < count; i++) {
//            // 寻找下一组头节点
//            ListNode* cur = group_head;
//            ListNode* newhead = nullptr;
//            for(int j = 0; j < k; j++) {
//                ListNode* next = cur->next;
//                cur->next = newhead;
//                newhead = cur;
//                cur = next;
//            }
//            if(prev_group_tail) {
//                prev_group_tail->next = newhead;
//            } else {
//                head = newhead;
//            }
//            prev_group_tail = group_head;
//            group_head = cur;
//        }
//        prev_group_tail->next = group_head;
//        return head;
//    }
//};

