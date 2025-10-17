// leetcode 283
// class Solution {
// public:
//     void moveZeroes(vector<int>& nums) {
//         int left = -1; 
//         int right = 0;
//         int len = nums.size();
//         while(right < len) {
//             if(nums[right]) {
//                 swap(nums[++left] , nums[right]);
//             }
//             right++;
//         }
//     }
// };

