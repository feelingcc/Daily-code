// class Solution {
//     bool isAnagram(int hash1[26] , int hash2[26]) {
//         for(int i = 0; i < 26; i++) {
//             if(hash1[i] != hash2[i])
//                 return false;
//         }
//         return true;
//     }
// public:
//     vector<int> findAnagrams(string s, string p) {
//         // 判断异位词的思路：每个字母的出现次数是一样的，通过哈希表判断
//         int left = 0 , right = 0;
//         vector<int> ans;
//         int hash1[26] = {0} , hash2[26] = {0};
//         // 1. 将p入哈希表
//         for(auto c : p)
//             hash2[c - 'a']++;
//         // 2. 滑动窗口
//         while(right < s.size()) {
//             // 2.1 进窗口
//             hash1[s[right++] - 'a']++;
//             // 2.2 判断窗口是否合法，不合法连续出窗口 由于此题的滑动窗口是固定的 所以只会超出一个元素，因此使用if
//             if(right - left > p.size()) {
//                 hash1[s[left++] - 'a']--;
//             }
//             // 2.3更新结果
//             if(isAnagram(hash1 , hash2))
//             {
//                 hash1[s[left] - 'a']--;
//                 ans.emplace_back(left++);
//             }
                
//         }
//         return ans;
//     }
// };