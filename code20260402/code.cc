//class Solution {
//public:
//    int massage(vector<int>& nums) {
//        // dp[i]表示到达i位置时，最大的预约时长，但是当前i位置有两个状态，一个是接一个是不接，因此继续细化
//        // f[i]表示到达i位置时，接受 i 位置的预约请求，最大的预约时长
//        // g[i]表示到达i位置时，不接受 i 位置的预约请求，最大的预约时长
//        int n = nums.size();
//        if(n == 0) return 0;
//        vector<int> f(n) , g(n);
//        f[0] = nums[0];
//        for(int i = 1; i < n; i++) {
//            f[i] = g[i - 1] + nums[i];
//            g[i] = max(f[i - 1] , g[i - 1]);
//        }
//        return max(f[n - 1] , g[n - 1]);
//    }
//};
