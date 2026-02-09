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
//public:
//    void reorderList(ListNode* head) {  
//        // 1. 快慢指针找中间元素
//        ListNode* slow = head , *fast = head;
//        while(fast && fast->next) {
//            slow = slow->next;
//            fast = fast->next->next;
//        }
//        // 2. slow位置后的节点逆置
//        ListNode* cur = slow->next;
//        slow->next = nullptr;
//        while(cur) {
//            ListNode* next = cur->next;
//            cur->next = slow;
//            slow = cur;
//            cur = next;
//        }
//        // 3. 重排
//        cur = new ListNode(-1);
//        ListNode* tail = cur;
//        fast = head;
//        while(fast && slow) {
//            if(fast == slow) {
//                tail->next = fast;
//                break;
//            }
//            ListNode* fastNext = fast->next;
//            ListNode* slowNext = slow->next;
//            tail->next = fast;
//            tail->next->next = slow;
//            tail = slow;
//            fast = fastNext;
//            slow = slowNext;
//        }
//        
//    }
//};
