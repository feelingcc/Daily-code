// class Solution {
// public:
//     int findMin(vector<int>& nums) {
//         if(nums.back() > nums.front()) return nums[0];
//         // 两段升序，可以通过最左边的数或最右边的数进行参考
//         int left = 0, right = nums.size() - 1;
//         while(left < right) {
//             int mid = left + (right - left) / 2;
//             if(nums[mid] >= nums[0]) left = mid + 1;
//             else right = mid;
//         }
//         return nums[left];
//     }
// };

// class Solution {
// public:
//     int takeAttendance(vector<int>& records) {
//         // 二段性：元素与下标之间的二段性
//         int left = 0 , right = records.size() - 1;
//         while(left < right) {
//             int mid = left + (right - left + 1) / 2;
//             if(records[mid] == mid) left = mid;
//             else right = mid - 1;
//         }
//         return left == records[left] ? left + 1 : left;
//     }
// };