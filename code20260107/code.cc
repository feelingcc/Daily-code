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
//    int mod = 1e9 + 7;
//    int dfs(TreeNode* root) {
//        if(root == nullptr) return 0;
//        return dfs(root->left) + dfs(root->right) + root->val;
//    }
//    int postorder(TreeNode* root , int total , long long& ans) {
//        if(root == nullptr) return 0;
//        int subSum = postorder(root->left , total , ans) + postorder(root->right , total , ans) + root->val;
//        if(ans < (total - subSum) * (long long)subSum)
//            ans = (total - subSum) * (long long)subSum;
//        return subSum;
//    }
//    int maxProduct(TreeNode* root) {
//        // 第一次dfs计算所有节点的和
//        int sum = dfs(root);
//        long long ans = 0;
//        // 第二次dfs使用后序遍历删除一条边
//        postorder(root , sum , ans);
//        return ans % mod;
//    }
//};
