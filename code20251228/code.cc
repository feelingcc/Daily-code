// leetcode 30.串联所有单词的子串
// class Solution {
//     bool isAnagram(unordered_map<string, int>& hash1 , unordered_map<string, int>& hash2) {
//         for (auto kv : hash2) {
//             auto iter = hash1.find(kv.first);
//             if (iter == hash1.end() || iter->second != kv.second) {
//                 return false;
//             }
//         }
//         return true;
//     }

// public:
//     vector<int> findSubstring(string s, vector<string>& words) {
//         // 滑动窗口 + 哈希表 （字符串异位词）
//         int sep = words[0].size();
//         unordered_map<string, int> hash2;
//         // 将 words 数组中所有单词统计到哈希表中
//         for (auto str : words) {
//             hash2[str]++;
//         }
//         vector<int> ans; // 统计结果的容器
//         // sep步划分，需要 sep 次滑动窗口
//         for(int i = 0; i < sep; i++) {
//             int left = i, right = i;
//             unordered_map<string, int> hash1;
//             while (right + sep <= s.size()) {
//                 string sub = s.substr(right, sep);
//                 hash1[sub]++;//  进窗口
//                 right += sep;
//                 // 不需要连续的出窗口，因为窗口的大小是固定的
//                 if (right - left > sep * words.size()) {
//                     string sub = s.substr(left, sep);
//                     hash1[sub]--;
//                     left += sep;
//                 }
//                 if (right - left == sep * words.size()) {
//                     if(isAnagram(hash1 , hash2)) {
//                         ans.emplace_back(left);
//                         string sub = s.substr(left, sep);
//                         hash1[sub]--;
//                         left += sep;
//                     }
//                 }
//             }
//         }
//         return ans;
//     }
// };