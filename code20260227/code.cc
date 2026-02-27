/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
//class Solution {
//public:
//    int widthOfBinaryTree(TreeNode* root) {
//        // 利用堆下标的特性去求解该问题，可以避免计算中间的空节点
//        queue<pair<TreeNode* , unsigned int>> q;
//        q.push({root , 0});
//        unsigned int ans = 0;
//        while(!q.empty()) {
//            int levelCount = q.size();
//            ans = max(ans , q.back().second - q.front().second + 1);
//            for(int i = 0; i < levelCount; i++) {
//                if(q.front().first->left) {
//                    q.push({q.front().first->left , 2 * q.front().second + 1});
//                }
//                if(q.front().first->right) {
//                    q.push({q.front().first->right , 2 * q.front().second + 2});
//                }
//                q.pop();
//            }
//        }
//        return ans;
//    }
//};
