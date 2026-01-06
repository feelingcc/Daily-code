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
//    int maxLevelSum(TreeNode* root) {
//        // 层序遍历
//        queue<TreeNode*> q;
//        q.push(root);
//        int level = 0;
//        int maxLevelSum = INT_MIN , minLevel = 1;
//        while(q.size()) {
//            level++;
//            int sz = q.size();
//            int sum = 0;
//            while(sz--) {
//                TreeNode* node = q.front();
//                q.pop();
//                sum += node->val;
//                if(node->left) q.push(node->left);
//                if(node->right) q.push(node->right);
//            }
//            if(sum > maxLevelSum) {
//                maxLevelSum = sum;
//                minLevel = level;
//            }
//        }
//        return minLevel;
//    }
//}
//
//;class Solution {
//public:
//    vector<int> productExceptSelf(vector<int>& nums) {
//        // 预处理两个前缀和数组
//        // f[i]: [0 , i - 1]所有元素的乘积
//        // g[i]: [i + 1 , n - 1]所有元素的乘积
//        int n = nums.size();
//        vector<int> f(n , 1) , g(n , 1);
//        for(int i = 1; i < n; i++) f[i] = f[i - 1] * nums[i - 1];
//        for(int i = n - 2; i >= 0; i--) g[i] = g[i + 1] * nums[i + 1];
//        vector<int> ans(n);
//        for(int i = 0; i < n; i++)
//            ans[i] = f[i] * g[i];
//        return ans;
//    }
//};
//class Solution {
//public:
//    int subarraySum(vector<int>& nums, int k) {
//        unordered_map<int , int> hash; // [前缀和，出现的次数]
//        hash[0]++;  // dp[i] == k的情况
//        // dp[i] - dp[j - 1] == k 说明存在和为k的子数组
//        int sum = 0;
//        int ans = 0;
//        for(auto val : nums) {
//            sum += val;
//            if(hash.count(sum - k)) ans += hash[sum - k];
//            hash[sum]++;
//        }
//        return ans;
//    }
//};
