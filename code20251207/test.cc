// class Solution {
// public:
//     int lengthOfLongestSubstring(string s) {
//         // 滑动窗口 + 哈希表
//         int left = 0 , right = 0;
//         // 绝对映射
//         int hash[128] = {0};  
//         int ans = 0;
//         while(right < s.size()) {
//             // 进窗口
//             hash[s[right]]++;
//             while(hash[s[right]] > 1)
//                 hash[s[left++]]--;
//             ans = max(ans , right - left + 1);
//             right++;
//         }
//         return ans;
//     }
// };