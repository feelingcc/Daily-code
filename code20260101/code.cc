// class Solution {
// public:
//     vector<int> plusOne(vector<int>& digits) {
//         // 2023年自己的代码“不够快，更不够狠”
//         int n = digits.size();
//         for(int i = n - 1; i >= 0; i--) {
//             digits[i]++;
//             digits[i] %= 10;
//             if(digits[i] != 0)
//                 return digits;
//         }
//         vector<int> ans(n + 1);
//         ans[0] = 1;
//         return ans;
//     }
// };

// class Solution {
// public:
//     vector<int> shortestSeq(vector<int>& big, vector<int>& small) {
//         unordered_map<int,int> small_hash , big_hash;
//         for(auto val : small) small_hash[val]++;
//         int min_len = INT_MAX , m = small.size();
//         vector<int> ans = {0 , static_cast<int>(big.size() - 1)};
//         for(int left = 0 , right = 0 , count = 0; right < big.size(); right++) {
//             big_hash[big[right]]++;
//             if(small_hash.count(big[right]) && big_hash[big[right]] == 1) count++;
//             // 出窗口
//             while(count == m) {
//                 // 更新结果
//                 if(right - left + 1 < min_len) {
//                     ans[0] = left;
//                     ans[1] = right;
//                     min_len = right - left + 1;
//                 }
//                 // 
//                 big_hash[big[left]]--;
//                 if(small_hash.count(big[left]) && big_hash[big[left]] == 0) count--;
//                 left++;
//             }
//         }
//         return min_len == INT_MAX ? vector<int>() : ans;
//     }
// };