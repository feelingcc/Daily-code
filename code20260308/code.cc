//class Solution {
//public:
//    int maxDistance(vector<vector<int>>& grid) {
//        // 正难则反，从所有的1作为起点开始扩散，最后寻找数组的最大值即可
//        int n = grid.size();
//        int dx[4] = {0 , 0 , 1,  -1};
//        int dy[4] = {1 , -1 , 0 , 0};
//        queue<pair<int,int>> q;
//        for(int i = 0; i < n; i++)
//            for(int j = 0; j < n; j++)
//                if(grid[i][j])
//                    q.push({i , j});
//        int ans = -1;
//        while(q.size()) {
//            auto [a , b] = q.front();
//            q.pop();
//            for(int i = 0; i < 4; i++) {
//                int x = a + dx[i] , y = b + dy[i];
//                if(x >= 0 && x < n && y >= 0 && y < n && grid[x][y] == 0) {
//                    grid[x][y] = grid[a][b] + 1;
//                    q.push({x , y});
//                    ans = max(ans , grid[x][y]);
//                }
//            }
//        }
//        return ans == -1 ? ans : ans - 1;
//    }
//};
//class Solution {
//public:
//    vector<vector<int>> highestPeak(vector<vector<int>>& isWater) {
//        // 从所有的0开始bfs
//        int m = isWater.size() , n = isWater[0].size();
//        int dx[4] = {0 , 0 , 1,  -1};
//        int dy[4] = {1 , -1 , 0 , 0};
//        queue<pair<int,int>> q;
//        for(int i = 0; i < m; i++)
//            for(int j = 0; j < n; j++)
//                if(isWater[i][j] == 1) {
//                   isWater[i][j] = 0;
//                   q.push({i , j});
//                } else {
//                    isWater[i][j] = -1;
//                }
//        while(q.size()) {
//            auto [a , b] = q.front();
//            q.pop();
//            for(int i = 0; i < 4; i++) {
//                int x = a + dx[i] , y = b + dy[i];
//                if(x >= 0 && x < m && y >= 0 && y < n && isWater[x][y] == -1) {
//                    isWater[x][y] = isWater[a][b] + 1;
//                    q.push({x , y});
//                }
//            }
//        }
//        return isWater;
//    }
//};
