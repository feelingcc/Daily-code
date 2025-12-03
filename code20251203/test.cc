// class Solution {
// public:
//     vector<vector<int>> fourSum(vector<int>& nums, int target) {
//         // 本质: 套一层三数之和
//         sort(nums.begin() , nums.end());
//         vector<vector<int>> res;
//         for(int i = nums.size() - 1; i > 2; i--) {
//             for(int j = i - 1; j > 1; j--) {
//                 int left = 0;
//                 int right = j - 1;
//                 while(left < right) {
//                     long long cur = (long long)nums[left] + nums[right] + nums[i] + nums[j];
//                     if(cur == target) {
//                         res.push_back({nums[left] , nums[right] , nums[i] , nums[j]});
//                         // left 和 right 去重
//                         left++;
//                         right--;
//                         while(left < right && nums[left] == nums[left - 1]) left++;
//                         while(left < right && nums[right] == nums[right + 1]) right--;
//                     }
//                     else if(cur < target) left++;
//                     else right--;
//                 }
//                 // j 去重
//                 while(j > 1 && nums[j] == nums[j - 1]) j--;
//             }
//             // i 去重
//             while(i > 2 && nums[i] == nums[i - 1]) i--;
//         }
//         return res;
//     }

// };