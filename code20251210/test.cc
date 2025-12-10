// class Solution {
// public:
//     int minOperations(vector<int>& nums, int x) {
//         // 正难则反
//         // |_a_|______ sum - x ______|_b_|
//         // a + b = x
//         // 转换为寻找和为 sum - x 的最长子数组
//         int target = -x;
//         for(auto val : nums) target += val;
//         int sum = 0;
//         int left = 0 , right = 0;
//         int ans = -1;
//         std::cout << target << std::endl;
//         while(right < nums.size()) {
//             sum += nums[right++];
//             while(left < right && sum > target) {
//                 sum -= nums[left++];
//             }
//             if(sum == target) {
//                 ans = max(ans , right - left);
//             }
//         }
//         return ans == -1 ? ans : nums.size() - ans;
//     }
// };

// class Solution {
// public:
//     int longestOnes(vector<int>& nums, int k) {
//         int left = 0;
//         int right = 0;
//         int ans = 0;
//         while(right < nums.size()) {
//             // 进窗口
//             if(nums[right]) {
//                 right++;
//             } else {
//                 if(k > 0) {
//                     right++;
//                     k--;
//                 } else {
//                     // 出窗口直到k > 0为止
//                     while(nums[left++] != 0);
//                     k++;
//                 }
//             }
//             // 更新结果
//             ans = max(ans , right - left);
//         }
//         return ans;
//     }
// };