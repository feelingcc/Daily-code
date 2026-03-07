//class Solution {
//public:
//    int numEnclaves(vector<vector<int>>& grid) {
//        // 多源最短路问题 正难则反 直接从边界的1开始向内扩散，最后在遍历一遍数组 若为1的位置就是结果
//        int dx[4] = {0 , 0, 1 , -1};
//        int dy[4] = {1 , -1 , 0 , 0};
//        int m = grid.size() , n = grid[0].size();
//        queue<pair<int,int>> q;
//        int ans = 0;
//        // 将所有的起点添加到队列中
//        for(int i = 0; i < m; i++)
//            for(int j = 0; j < n; j++)
//                if(i == 0 || j == 0 || i == m - 1 || j == n - 1)
//                    if(grid[i][j]) {
//                        q.push({i , j});
//                        grid[i][j] = 2;
//                    }
//        while(q.size()) {
//            auto [a , b] = q.front();
//            q.pop();
//            for(int i = 0; i < 4; i++) {
//                int x = a + dx[i] , y = b + dy[i];
//                if(x >= 0 && x < m && y >= 0 && y < n && grid[x][y] == 1) {
//                    q.push({x, y});
//                    grid[x][y] = 2;
//                }
//            }
//        }
//        for(int i = 0; i < m; i++)
//            for(int j = 0; j < n; j++)
//                if(grid[i][j] == 1)
//                    ans++;
//        return ans;
//    }
//};
//class Solution {
//public:
//    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
//        // 多源最短路问题 正难则反 如果将1当成起点，则无法得出结果，但是如果从0开始像外扩散，则很容易得出结果
//        int dx[4] = {0 , 0, 1 , -1};
//        int dy[4] = {1 , -1 , 0 , 0};
//        int m = mat.size() , n = mat[0].size();
//        vector<vector<bool>> check(m , vector<bool>(n));
//        vector<vector<int>> ans(m , vector<int>(n));
//        queue<pair<int,int>> q;
//        int step = 0;
//        // 将所有的起点添加到队列中
//        for(int i = 0; i < m; i++)
//            for(int j = 0; j < n; j++)
//                if(mat[i][j] == 0) {
//                    q.push({i , j});
//                    check[i][j] = true;
//                }
//        while(q.size()) {
//            int sz = q.size();
//            step++;
//            while(sz--) {
//                auto [a , b] = q.front();
//                q.pop();
//                for(int i = 0; i < 4; i++) {
//                    int x = a + dx[i] , y = b + dy[i];
//                    if(x >= 0 && x < m && y >= 0 && y < n && !check[x][y]) {
//                        q.push({x , y});
//                        check[x][y] = true;
//                        ans[x][y] = step;
//                    }
//                }
//            }
//        }
//        return ans;
//    }
//};
