// class Solution {
// public:
//     int totalFruit(vector<int>& fruits) {
//         int hash[100001] = {0};
//         int left = 0 , right = 0;
//         int ans = 0;
//         int kinds = 0;
//         while(right < fruits.size()) {
//             if(hash[fruits[right]] == 0) kinds++;   // 维护水果种类的数量   
//             hash[fruits[right++]]++;    // 进窗口
//             while(kinds > 2) {
//                 hash[fruits[left]]--;
//                 if(hash[fruits[left]] == 0)
//                     kinds--;
//                 left++;
//             }
//             ans = max(ans , right - left);
//         }
//         return ans;
//     }
// };