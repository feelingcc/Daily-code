////class Solution {
//public:
//    int findSubstringInWraproundString(string s) {
//        // dp[i]表示以i位置为结尾，所有子串中，在base字符串出现的非空子串
//        int n = s.size();
//        vector<int> dp(n , 1);
//        for(int i = 1; i < n; i++)
//            if((s[i - 1] + 1 == s[i]) || (s[i - 1] == 'z' && s[i] == 'a'))
//                dp[i] += dp[i - 1];
//
//        // 去重       
//        int hash[26] = {0};
//        for(int i = 0; i < n; i++)
//            hash[s[i] - 'a'] = max(hash[s[i] - 'a'] , dp[i]);
//        
//        // 统计
//        int sum = 0;
//        for(auto val : hash) sum += val;
//        return sum;
//    }
//};
