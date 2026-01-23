//class Solution {
//public:
//    int minNumberOfFrogs(string croakOfFrogs) {
//        string src = "croak";
//        int n = src.size();
//        vector<int> hash(n); // 对应 croak 的原始下标
//        unordered_map<int , int> index; // [x字符，x字符对应的下标]
//        for(int i = 0; i < n; i++) index[src[i]] = i;
//
//        for(auto c : croakOfFrogs) {
//            if(c == 'c') {
//                if(hash[n - 1] > 0) {
//                    hash[n - 1]--;
//                    
//                } 
//                hash[index[c]]++;
//            } else {
//                if(hash[index[c] - 1] > 0) {
//                    hash[index[c] - 1]--;
//                    hash[index[c]]++;
//                } else {
//                    return -1;
//                }
//            }
//        }
//        for(int i = 0; i < n - 1; i++) if(hash[i]) return -1;
//        return hash[n - 1];
//    }
//};
