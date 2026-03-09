//class Solution {
//public:
//    string alienOrder(vector<string>& words) {
//        // 使用嵌套哈希表存储图
//        unordered_map<char , unordered_set<char>> graph;
//        // 存 words 中出现的每个字符的入度，需要初始化
//        unordered_map<char , int> in;
//        for(auto& word : words)
//            for(auto c : word)
//                in[c] = 0;
//        // 将words中的信息转换为图
//        for(int i = 0; i < words.size(); i++) {
//            for(int j = i + 1; j < words.size(); j++) {
//                // 找两个字符串第一个不相同的字符
//                int k = 0;
//                while(k < words[i].size() && k < words[j].size() && words[i][k] == words[j][k])
//                    k++;
//                // 检查非法情况：如果较长的字符串排在较短的字符串前面
//                if(k == words[j].size() && k < words[i].size()) {
//                    return "";
//                }
//                // 如果 k 越界了两个字符串之中任意一个字符串，则说明这次的比较的没有意义，区分不出来顺序
//                if(k < words[i].size() && k < words[j].size()) {
//                    // words[i][k] -> words[j][k]
//                    // 去除重复添加的边和入度信息
//                    if(!graph.count(words[i][k]) || !graph[ words[i][k] ].count( words[j][k] )){
//                        graph[ words[i][k] ].insert( words[j][k] );
//                        in[ words[j][k] ]++;
//                    }   
//                }
//            } 
//        }
//        // 拓扑排序
//        queue<char> q;
//        // 将所有入度为0的节点添加到队列中
//        for(auto [v , indeg] : in)
//            if(indeg == 0)
//                q.push(v);
//        string ans;
//        while(q.size()) {
//            char v = q.front(); q.pop();
//            ans += v;
//            // 将 v 顶点的领接顶点的入度减1
//            for(auto adjv : graph[v]) {
//                in[adjv]--;
//                if(in[adjv] == 0)
//                    q.push(adjv);
//            }
//        }
//        // 检查是否能拓扑排序
//         for(auto [v , indeg] : in)
//            if(indeg)
//                return "";
//        return ans;
//    }
//};
//class Solution {
//public:
//    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
//        // 拓扑排序的思想
//        // 首先借助容器快速建图
//        unordered_map<int , vector<int>> graph; // 邻接表
//        vector<int> in(numCourses); // 存储每个节点的入度
//        for(auto & v : prerequisites) {
//            graph[v[1]].push_back(v[0]); // v[1] -> v[0]
//            in[v[0]]++;
//        }
//        // 将入度为0的顶点添加到队列中
//        queue<int> q;
//        for(int i = 0; i < numCourses; i++)
//            if(in[i] == 0)
//                q.push(i);
//        // 拓扑排序
//        while(q.size()) {
//            int v = q.front(); q.pop();
//            // 将v连接的所有顶点的入度减一
//            for(auto v : graph[v]) {
//                in[v]--;
//                if(in[v] == 0)
//                    q.push(v);
//            }
//        }
//        // 拓扑排序结束后，应该入度全为0
//        for(auto val : in)
//            if(val)
//                return false;
//        return true;
//    }
//};
//class Solution {
//public:
//    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
//        // 拓扑排序的思想
//        // 首先借助容器快速建图
//        unordered_map<int , vector<int>> graph; // 邻接表
//        vector<int> in(numCourses); // 存储每个节点的入度
//        for(auto & v : prerequisites) {
//            graph[v[1]].push_back(v[0]); // v[1] -> v[0]
//            in[v[0]]++;
//        }
//        // 将入度为0的顶点添加到队列中
//        queue<int> q;
//        for(int i = 0; i < numCourses; i++)
//            if(in[i] == 0)
//                q.push(i);
//        // 拓扑排序
//        vector<int> ans;
//        while(q.size()) {
//            int v = q.front(); q.pop();
//            ans.emplace_back(v);
//            // 将v连接的所有顶点的入度减一
//            for(auto v : graph[v]) {
//                in[v]--;
//                if(in[v] == 0)
//                    q.push(v);
//            }
//        }
//        // 拓扑排序结束后，应该入度全为0
//        for(auto val : in)
//            if(val)
//                return {};
//        return ans;
//    }
//};
