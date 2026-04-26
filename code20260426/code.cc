//class Solution {
//public:
//    int lengthOfLIS(vector<int>& nums) {
//        //dp[i]表示以i结尾，所有的子序列中，严格递增子序列最长的
//        int n = nums.size();
//        vector<int> dp(n , 1);
//        int ans = 1;
//        for(int i = 1; i < n; i++) {
//            for(int j = 0; j < i; j++)
//                if(nums[i] > nums[j]) dp[i] = max(dp[i] , dp[j] + 1);
//            ans = max(ans , dp[i]);
//        }
//        return ans;
//    }
//};
