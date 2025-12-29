// class Solution {
// public:
//     string minWindow(string s, string t) {
//         int left = 0 , right = 0;
//         unordered_map<char , int> hash1 , hash2;
//         // t中所有字符进哈希
//         for(auto c : t) hash2[c]++;
//         int count = 0;  // 记录有效字符的种类
//         int minLen = INT_MAX , begin = -1;
//         int m = hash2.size();
//         while(right < s.size()) {
//             // 进窗口
//             hash1[s[right]]++;
//             // 维护count
//             if(hash1[s[right]] == hash2[s[right]]) count++;
//             right++;
//             // 若条件满足，则一直出窗口
//             while(count == m) {
//                 // 更新结果
//                 if(right - left < minLen) {
//                     minLen = right - left;
//                     begin = left;
//                 }
//                 // 维护count
//                 if(hash1[s[left]] == hash2[s[left]])
//                     count--;
//                 // 出窗口
//                 hash1[s[left++]]--;
//             }
//         }
//         return minLen == INT_MAX ? "" : s.substr(begin , minLen);
//     }
// };