//class Solution {
//public:
//    /*
//        1 0  12
//        8 4  14
//        2 10 15
//        1 - 2 的最短路 + 2 - 4 的最短路 + 4 - 8 的最短路 ... 若干个最短路相加
//    */
//    int cutOffTree(vector<vector<int>>& forest) {
//        // 注意题目要求，必须要按从低到高的顺序去砍
//        // 将非0元素的下标存起来，排序，排序规则按照下标的元素大小
//        vector<pair<int,int>> preprocessing;
//        m = forest.size() , n = forest[0].size();
//        for(int i = 0; i < m; i++)
//            for(int j = 0; j < n; j++)
//                if(forest[i][j] > 1) // 注意题目，比1大的数才代表有树，1是地面不用砍
//                    preprocessing.push_back({i , j});
//        sort(preprocessing.begin() , preprocessing.end() , 
//            [&forest](const pair<int,int>& p1 , const pair<int,int>& p2)->bool{
//                return forest[p1.first][p1.second] < forest[p2.first][p2.second];
//            }
//        );
//        int ans = 0;
//        pair<int,int> start = {0 , 0};
//        for(auto & pv : preprocessing) {
//            int res = bfs(forest , start , pv);
//            if(res == -1) return res;
//            ans += res;
//            start = pv;
//        }
//        return ans;
//    }
//    int dx[4] = {0 , 0 , 1 , -1};
//    int dy[4] = {1 , -1, 0, 0 };
//    int m , n;
//    int bfs(vector<vector<int>>& forest , const pair<int,int>& start , const pair<int,int>& end) {
//        if(start == end) return 0;
//        bool check[51][51] = {false};
//        queue<pair<int,int>> q;
//        q.push(start);
//        check[start.first][start.second] = true;
//        int step = 0;
//        while(q.size()) {
//            int sz = q.size();
//            step++;
//            while(sz--) {
//                auto [a , b] = q.front();
//                q.pop();
//                for(int i = 0; i < 4; i++) {
//                    int x = a + dx[i] , y = b + dy[i];
//                    if(x >= 0 && x < m && y >= 0 && y < n && forest[x][y] && !check[x][y]) {
//                        if(x == end.first && y == end.second) return step;
//                        q.push({x , y});
//                        check[x][y] = true;
//                    }
//                }
//            }
//        }
//        return -1;
//    }
//};
//class Solution {
//public:
//    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
//        // 最短路问题
//        unordered_set<string> word_hash(wordList.begin() , wordList.end());
//        if(!word_hash.count(endWord)) return 0;
//        unordered_set<string> check; //  记录已经走过的路，防止死循环
//        queue<string> q;
//        q.push(beginWord);
//        check.insert(beginWord);
//        int step = 1;
//        while(q.size()) {
//            int sz = q.size();
//            step++; // 队列中有多少元素，代表下一步有多少变化
//            while(sz--) {
//                string src_str = q.front();
//                q.pop();
//                for(int i = 0; i < beginWord.size(); i++) {
//                    string change_after_str = src_str;
//                    for(char c = 'a'; c <= 'z'; c++) {
//                        change_after_str[i] = c;
//                        if(word_hash.count(change_after_str) && !check.count(change_after_str)) {
//                            if(change_after_str == endWord) return step;
//                            q.push(change_after_str);
//                            check.insert(change_after_str);
//                        }
//                    }
//                }
//            }
//        }
//        return 0;
//    }
//};
