//class Solution {
//public:
//    int numberOfArithmeticSlices(vector<int>& nums) {
//        // dp[i]表示以i位置为结尾，所有子数组中，为等差数组的子数组个数
//        int n = nums.size();
//        vector<int> dp(n);
//        int sum = 0;
//        for(int i = 2; i < n; i++) {
//            if(nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2])
//                dp[i] = dp[i - 1] + 1;
//            sum += dp[i];
//        }
//        return sum;
//    }
//};
