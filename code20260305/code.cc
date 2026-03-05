//class Solution {
//public:
//    int minMutation(string startGene, string endGene, vector<string>& bank) {
//        unordered_map<string , int> bank_hash;
//        for(auto& val : bank) bank_hash[val]++;
//        if(!bank_hash.count(endGene)) return -1;
//        string changes = "ACGT";
//        unordered_map<string , int> check;
//        queue<string> q;
//        q.push(startGene);
//        check[startGene]++;
//        int step = 0;
//        while(q.size()) {
//            int sz = q.size();
//            step++;
//            for(int k = 0; k < sz; k++) {
//                string str = q.front();
//                q.pop();
//                for(int i = 0; i < 8; i++) {
//                    for(auto c : changes) {
//                        // if(c != str[i]) {
//                            string changeAfter = str;
//                            changeAfter[i] = c;
//                            if(bank_hash.count(changeAfter) && !check.count(changeAfter)) {
//                                if(endGene == changeAfter) return step;
//                                check[changeAfter] = true;
//                                q.push(changeAfter);
//                            }
//                        // }
//                    }
//                }
//            } 
//        }
//        return -1;
//    }
//};
//
//class Solution {
//public:
//    int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
//        // 最短路问题
//        int dx[4] = {0 , 0, 1, -1};
//        int dy[4] = {1, -1, 0 , 0};
//        bool check[101][101] = {false};
//        queue<pair<int,int>> q;
//        q.push({entrance[0] , entrance[1]});
//        check[entrance[0]][entrance[1]] = true;
//        int step = 0;
//        while(q.size()) {
//            int sz = q.size();
//            step++;
//            for(int i = 0; i < sz; i++) {
//                auto [a , b] = q.front();
//                q.pop();
//                for(int j = 0; j < 4; j++) {
//                    int x = a + dx[j] , y = b + dy[j];
//                    if(x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] == '.' && !check[x][y]) {
//                        if(x == 0 || x == maze.size() - 1 || y == 0 || y == maze[0].size() - 1) return step;
//                        q.push({x , y});
//                        check[x][y] = true;
//                    }
//                }
//            }
//        }
//        return -1;
//    }
//};
//
