// // leetcode 371. 两整数之和
// class Solution {
// public:
//     int getSum(int a, int b) {
//         // ^ 异或本质是无进位相加
//         // 先&一下看一下哪些是需要进位的
//         return ((a & b) << 1) + (a ^ b);
//     }
// };

// // leetcode 面试题判断字符是否唯一（不使用数据结构哦）
// class Solution {
// public:
//     bool isUnique(string astr) {
//         // 使用位图的思想
//         int hash = 0;
//         for(auto c : astr) {
//             if((hash >> c - 'a') & 1) return false;
//             hash |= 1 << c - 'a';
//         }
//         return true;
//     }
// };

// // leetcode 268.丢失的数字
// class Solution {
// public:
//     int missingNumber(vector<int>& nums) {
//         int x = 0;
//         int n = nums.size();
//         for(int i = 0; i <= n; i++) x ^= i;
//         for(auto val : nums) x ^= val;
//         return x;
//     }
// };

// // leetcode  只出现一次的数字2
// class Solution {
// public:
//     int singleNumber(vector<int>& nums) {
//         // 每一位无非就是出现 3n次 + 0/1 次 所以只要 %3 就能知道出现一次数字的那一位是否为0还是为1
//         int res = 0;
//         for(int i = 0; i < 32; i++) {
//             int bit = 0;
//             for(auto val : nums) {
//                 bit += val >> i & 1;
//             }
//             res |= bit % 3 << i;
//         }
//         return res;
//     }
// };

// // leetcode 消失的两个数字（分组异或）
// class Solution {
// public:
//     vector<int> missingTwo(vector<int>& nums) {
//         // 分组异或
//         int n = nums.size() + 2;
//         int x = 0;
//         for(int i = 1; i <= n; i++)  x ^= i;
//         for(auto val : nums) x ^= val;
//         // x此时为消失的两个数字的相异位，随便取出一位，按该位分组即可
//         int diff = x & (-x);    // 取出x二进制位中最右侧的是1的bit
//         vector<int> ans(2);
//         for(int i = 1; i <= n; i++)
//             if(i & diff) ans[0] ^= i;
//         for(auto val : nums)
//             if(val & diff) ans[0] ^= val;
//         ans[1] = ans[0] ^ x;
//         return ans;
//     }
// };