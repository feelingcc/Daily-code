//class Solution {
//public:
//    string longestCommonPrefix(vector<string>& strs) {
//        // 直接使用第一个字符串与后续字符串对比
//        for(int i = 0; i < strs[0].size(); i++) {
//            char c = strs[0][i];
//            for(int j = 1; j < strs.size(); j++) {
//                // 两种情况：1. 第一个字符串是最短的
//                //          2.后面有的字符串比第一个字符串还短
//                if(i >= strs[j].size() || strs[j][i] != c)
//                    return strs[0].substr(0 , i);
//            }
//        }
//        return strs[0];
//    }
//};
