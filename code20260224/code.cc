//class Solution {
//public:
//    string solve(string & s) {
//        string ans;
//        for(auto c : s) {
//            if(c == '#') {
//                if(!ans.empty()) ans.pop_back();
//            }
//            else ans += c;
//        }
//        return ans;
//    }
//    bool backspaceCompare(string s, string t) {
//        return solve(s) == solve(t);
//    }
//};
//
//class Solution {
//public:
//    int calculate(string s) {
//        stack<int> st;
//        char op = '+';
//        int i = 0 , n = s.size();
//        while(i < n) {
//            if(s[i] == ' ') i++;
//            else if(s[i] >= '0' && s[i] <= '9') {
//                int temp = 0;
//                while(i < n && s[i] >= '0' && s[i] <= '9') temp = temp * 10 + (s[i++] - '0');
//                if(op == '+') st.push(temp);
//                else if(op == '-') st.push(-temp);
//                else if(op == '*') st.top() *= temp;
//                else st.top() /= temp;
//            } else {
//                op = s[i++];
//            }
//        }
//
//        int ans = 0;
//        while(!st.empty()) {
//            ans += st.top();
//            st.pop();
//        }
//
//        return ans;
//    }
//};
