//class Solution {
//public:
//    string convert(string s, int numRows) {
//        if(numRows == 1) return s;
//        /*
//            0         6         12
//            1      5  7     11  13
//            2   4     8  10     14
//            3         9         15  
//        */
//        // 根据下标找规律
//        int n = s.size();
//        int d = 2 * numRows - 2;// 公差
//        string ans;
//        // 处理第一行
//        for(int i = 0; i < n; i += d) ans += s[i];
//        // 处理中间行
//        for(int k = 1; k < numRows - 1; k++) {
//            for(int i = k , j = d - i; i < n || j < n; i += d , j += d) {
//                if(i < n) ans += s[i];
//                if(j < n) ans += s[j];
//            }
//        }
//        // 处理最后一行
//        for(int i = numRows - 1; i < n; i += d) ans += s[i];
//        return ans;
//    }
//};
