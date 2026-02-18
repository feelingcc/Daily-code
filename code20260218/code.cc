//class Solution {
//public:
//    string multiply(string num1, string num2) {
//        // 有可能一个数和 0 相乘 结果就可能是多个"00000" 但实际结果只有一个，处理前导0
//        if(num1[0] == '0' || num2[0] == '0') return "0";
//        // 123
//        // 456
//        /*
//              6  12 18
//            5 10 15
//          4 8 12
//          0 1 2  3  4 
//        */
//        // 无进位相加的结果是 m + n - 1 这么多位数，但实际的结果可能是 m + n
//        // 无进位相加后代表落在 i + j 的位置
//        // 4 13 28 27 18
//        // 56088
//        int n = num1.size() , m = num2.size();
//        vector<int> temp(n + m - 1);
//        for(int i = m - 1; i >= 0; i--) {
//            for(int j = n - 1; j >= 0; j--) {
//                temp[i + j] += (num2[i] - '0') * (num1[j] - '0');
//            }
//        }
//        // for(auto v : temp) cout << v << " ";
//        // 处理无进位相加
//        int i = n + m - 2;
//        int t = 0;  // 记录进位
//        string ans;
//        while(i >= 0 || t) {
//            int x = i >= 0 ? temp[i--] : 0;
//            t += x;
//            ans += to_string(t % 10);
//            t /= 10;
//        }
//        reverse(ans.begin() , ans.end());
//        return ans;
//    }
//};
//
//class Solution {
//public:
//    string addBinary(string a, string b) {
//        int l = a.size() - 1 , r = b.size() - 1;
//        int t = 0;
//        string ans;
//        while(l >= 0 || r >= 0 || t) {
//            int x = l >= 0 ? a[l--] - '0' : 0;
//            int y = r >= 0 ? b[r--] - '0' : 0;
//            t += x + y;
//            ans += t % 2 + '0';
//            t /= 2;
//        }
//        reverse(ans.begin() , ans.end());
//        return ans;
//    }
//};
