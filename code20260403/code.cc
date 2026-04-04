//class Solution {
//public:
//    int rob(vector<int>& nums) {
//        // dp[i]表示到达i位置时，能偷窃盗的最高金额
//        // 但是当前i位置有两种状态，偷或者不偷
//        // f[i]表示到达i位置时，偷取i位置，能偷窃的最高金额
//        // g[i]表示到达i位置时，不偷取i位置，能偷窃的最高金额
//        int n = nums.size();
//        vector<int> f(n) , g(n);
//        f[0] = nums[0];
//        for(int i = 1; i < n; i++) {
//            f[i] = g[i - 1] + nums[i];
//            g[i] = max(f[i - 1] , g[i - 1]);
//        }
//        return max(f[n - 1] , g[n - 1]);
//    }
//};
//
//class Solution {
//public:
//    int rob(vector<int>& nums) {
//        // 将环型问题转换为线性问题，分情况讨论
//        // 若第一个位置偷，则第二个位置和最后一个位置不能偷，即 [2 , n - 2] 做一次打家劫舍I
//        // 若第一个位置不偷，则剩余位置随便，即 [1 , n - 1] 做一次打家劫舍I
//        int n = nums.size();
//        return max(nums[0] + rob1(nums , 2 , n - 2) , rob1(nums , 1, n - 1));
//    }
//    int rob1(vector<int>& nums , int left , int right) {
//        if(left > right) return 0;
//        vector<int> f(nums.size()) , g(nums.size());
//        f[left] = nums[left];
//        for(int i = left + 1; i <= right; i++) {
//            f[i] = g[i - 1] + nums[i];
//            g[i] = max(f[i - 1] , g[i - 1]);
//        }
//        return max(f[right] , g[right]);
//    }
//};
