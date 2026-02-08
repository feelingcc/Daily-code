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
// 1 -> 2 -> 3
// -1 -> 2 -> 1
//class Solution {
//public:
//    ListNode* swapPairs(ListNode* head) {
//        if(!head || !head->next) return head;
//        ListNode* ans = new ListNode(-1);
//        ListNode* tail = ans;
//        ListNode* l = head , *r = head->next , *n = r->next;
//        while(l && r) {
//            tail->next = r;
//            if(r) r->next = l;
//            else tail->next = l;
//            tail = l;
//            l = n;
//            if(l) r = l->next;
//            if(r) n = r->next;
//        }
//        if(l) tail->next = l;
//        else tail->next = nullptr;
//        return ans->next;
//    }
//};
