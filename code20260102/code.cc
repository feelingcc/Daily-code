// class Solution {
// public:
//     int mySqrt(int x) {
//         if(x == 1) return 1;
//         int left = 0 , right = x / 2;
//         while(left < right) {
//             long long mid = left + (right - left + 1) / 2;
//             if(mid * mid <= x) left = mid;
//             else right = mid - 1;
//         }
//         return left;
//     }
// };

// class Solution {
// public:
//     int searchInsert(vector<int>& nums, int target) {
//         int left = 0 , right = nums.size() - 1;
//         while(left < right) {
//             int mid = left + (right - left) / 2;
//             if(nums[mid] >= target) righta = mid;
//             else left = mid + 1;
//         }
//         return nums[left] < target ? nums.size() : left;
//     }
// };