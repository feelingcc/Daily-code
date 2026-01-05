// class Solution {
// public:
//     int pivotIndex(vector<int>& nums) {
//         // 预处理前缀和数组
//         // 1. leftSum[i]: 表示[0 , i - 1]所有元素的和
//         // 2. rightSum[i]: 表示[n - 1 , i + 1]所有元素的和
//         int n = nums.size();
//         vector<int> leftSum(n) , rightSum(n);
//         for(int i = 1; i < n; i++) leftSum[i] = leftSum[i - 1] + nums[i - 1];
//         for(int i = n - 2; i >= 0; i--) rightSum[i] = rightSum[i + 1] + nums[i + 1];

//         // 从左向右遍历
//         for(int i = 0; i < n; i++)
//             if(leftSum[i] == rightSum[i]) return i;
        
//         return -1;
//     }
// };