// class Solution {
// public:
//     int minSubArrayLen(int target, vector<int>& nums) {
//         int ans = INT_MAX;
//         int sum = 0;
//         int left = 0;
//         int right = 0;
//         while(right < nums.size()) {
//             // 进窗口
//             sum += nums[right];

//             // 可能会连续的出窗口
//             while(sum >= target) {
//                 ans = min(right - left + 1 , ans);    // 更新长度
//                 // 出窗口
//                 sum -= nums[left++];
//             }

//             right++;
//         }

//         return ans == INT_MAX ? 0 : ans;
//     }
// };