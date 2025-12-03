// class Solution {
// public:
//     int bitsum(int n) {
//         int sum = 0;
//         while(n) {
//             int t = n % 10;
//             sum += t * t;
//             n /= 10;
//         }
//         return sum;
//     }
//     bool isHappy(int n) {
//         // 无论是否是快乐数，都会进入到一个环内
//         //  - 如果是快乐数，这个环中所有的数就都是1
//         //  - 如果不是快乐数，这个环就回回到之前出现过的数字
//         // 利用判断链表有环的思路   
//         int slow = n , fast = n;
//         while(true) {
//             fast = bitsum(bitsum(fast));
//             slow = bitsum(slow);
//             if(fast == slow) {
//                 return fast == 1 ? true : false;
//             }
//         }
//     }
// };


// class Solution {
// public:
//     int maxArea(vector<int>& height) {
//         int left = 0 , right = height.size() - 1;
//         int res = 0;
//         while(left < right) {
//             int t = min(height[left] , height[right]) * (right - left);
//             res = max(t , res);
//             if(height[left] < height[right])
//                 left++;
//             else
//                 right--;
//         }
//         return res;
//     }
// };

// class Solution {
// public:
//     int triangleNumber(vector<int>& nums) {
//         // 快速判断是三角形的方法
//         // 如果    a <= b <= c
//         // 只需判断 a + b > c 

//         int res = 0;
        
//         // 1.排序
//         sort(nums.begin() , nums.end());
        
//         // 2.利用单调性和上面的数学定理，先固定最大的数
//         for(int i = nums.size() - 1; i >= 2; i--) {
//             // 3. 在固定最大的数的左区间找三角形
//             int a = 0;
//             int b = i - 1;
//             while(a < b) {
//                 // a最小的数 b第二大的数 i最大的数
//                 if(nums[a] + nums[b] > nums[i]) {
//                     // 单调性 a下标右边的数 + nums[b] 也一定大于 nums[i]
//                     res += b - a;
//                     b--;
//                 } else {
//                     // 单调性 a下标++ 继续上述过程
//                     a++;
//                 }
//             }
//         }
        
//         return res;
//     }
// };