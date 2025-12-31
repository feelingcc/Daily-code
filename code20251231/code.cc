
// class Solution {
// public:
//     int longestSubstring(string s, int k) {
//         int ans = 0;
//         // i表示字符的种类
//         for(int i = 1; i <= 26; i++) {
//             int hash[26] = {0}; // 每次滑动窗口开始需要新的哈希表，防止数据污染
//             for(int left = 0 , right = 0 , count = 0 , kinds = 0 ; right < s.size(); right++) {
//                 // kinds 代表当前哈希表中的种类，count 表示当前窗口超过k个的字符种类
//                 // 1. 进窗口
//                 char in = s[right];
//                 if(hash[in - 'a'] == 0) kinds++;    // 维护kinds
//                 hash[in - 'a']++;
//                 if(hash[in - 'a'] == k) count++;    // 维护count
//                 // 2. 出窗口
//                 while(kinds > i) { // 当前窗口中的种类超出了当前寻找的至少有k个重复字符的种类
//                     char out = s[left];
//                     if(hash[out - 'a'] == k) count--;    // 维护count
//                     hash[out - 'a']--;
//                     if(hash[out - 'a'] == 0) kinds--;    // 维护kinds
//                     left++;
//                 }
//                 // debug
//                 if(i == 2) {
//                     cout << left << " " << right << " " << count << " " << kinds << " " << i << endl;
//                 }
//                 // 3. 更新结果
//                 if(kinds == i && count == kinds){
//                     ans = max(ans , right - left + 1);
//                     // cout << ans << " " << i << endl;
//                 }
//             }
//         }
//         return ans;
//     }
// };