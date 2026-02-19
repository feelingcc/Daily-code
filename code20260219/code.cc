//class Solution {
//    
//public:
//    string removeDuplicates(string s) {
//        // 用数组模拟栈结构，最好最终结果就是输出结果
//        string stack;
//        for(auto c : s) {
//            if(!stack.empty() && stack.back() == c) {
//                stack.pop_back();
//                continue;
//            }
//            stack.push_back(c);
//        }
//        return stack;
//    }
//};
