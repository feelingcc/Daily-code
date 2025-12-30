// class Solution {
// public:
//     vector<string> findRepeatedDnaSequences(string s) {
//         vector<string> ans;
//         unordered_map<string , int> hash;   // 记录 DNA 出现的次数
//         for(int left = 0 , right = 0; right < s.size(); right++) {
//             // 默认的right++就是进窗口
//             // 固定的窗口
//             if(right - left + 1 == 10) {
//                 // 当前DNA序列添加到哈希表中
//                 string key = s.substr(left , right - left + 1);
//                 hash[key]++;
//                 if(hash[key] == 2) ans.emplace_back(key);
//                 left++; // 出窗口
//             }
//         }
//         return ans;
//     }
// };