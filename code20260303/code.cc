//class Solution {
//    int m , n;
//    int dx[4] = {0 , 0, 1 , -1};
//    int dy[4] = {1, -1, 0 , 0};
//    void bfs(vector<vector<char>>& grid , int i , int j) {
//        queue<pair<int , int>> q;
//        q.push({i , j});
//        grid[i][j] = '#';
//        while(q.size()) {
//            auto [a, b] = q.front();
//            q.pop();
//            for(int i = 0; i < 4; i++) {
//                int x = a + dx[i] , y = b + dy[i];
//                if(x >= 0 && x < m && y >= 0 && y < n && grid[x][y] == 'O') {
//                    q.push({x , y});
//                    grid[x][y] = '#';
//                }
//            }
//        }
//    }
//public:
//    void solve(vector<vector<char>>& board) {
//        m = board.size() , n = board[0].size();
//        for(int i = 0; i < m; i++) {
//            for(int j = 0; j < n; j++) {
//                if(i == 0 || j == 0 || i == m - 1 || j == n - 1)
//                    if(board[i][j] == 'O')
//                        bfs(board , i , j);
//            }
//        }
//        for(int i = 0; i < m; i++) {
//            for(int j = 0; j < n; j++) {
//                if(board[i][j] == '#') board[i][j] = 'O';
//                else if(board[i][j] == 'O') board[i][j] = 'X';
//            }
//        }
//    }
//};
//
//class Solution {
//    int m , n;
//    int dx[4] = {0 , 0, 1 , -1};
//    int dy[4] = {1, -1, 0 , 0};
//    bool checks[51][51];
//    int ans = 0;
//
//    void bfs(vector<vector<int>>& grid , int i , int j) {
//        queue<pair<int , int>> q;
//        q.push({i , j});
//        checks[i][j] = true;
//        int count = 0;
//        while(q.size()) {
//            auto [a, b] = q.front();
//            q.pop();
//            count++;
//            for(int i = 0; i < 4; i++) {
//                int x = a + dx[i] , y = b + dy[i];
//                if(x >= 0 && x < m && y >= 0 && y < n && grid[x][y] == 1 && checks[x][y] == false) {
//                    q.push({x , y});
//                    checks[x][y] = true;
//                }
//            }
//        }
//        ans = max(ans , count);
//    }
//public:
//    int maxAreaOfIsland(vector<vector<int>>& grid) {
//        m = grid.size() , n = grid[0].size();
//        for(int i = 0; i < m; i++) {
//            for(int j = 0; j < n; j++) {
//                if(grid[i][j] == 1 && !checks[i][j]) {
//                    bfs(grid , i , j);
//                }
//            }
//        }
//        return ans;
//    }
//};
//
//class Solution {
//    int dx[4] = {0 , 0, 1 , -1};
//    int dy[4] = {1 , -1 , 0 , 0};
//    void bfs(vector<vector<char>>& grid , vector<vector<bool>>& checks, int row , int col) {
//        queue<pair<int , int>> q;
//        q.push({row , col});
//        checks[row][col] = true;
//        while(q.size()) {
//            auto [x , y] = q.front();
//            q.pop();
//
//            for(int i = 0; i < 4; i++) {
//                if(x + dx[i] >= 0 && x + dx[i] < grid.size() && y + dy[i] >= 0 && y + dy[i] < grid[0].size() && grid[x + dx[i]][y + dy[i]] == '1' && !checks[x + dx[i]][y + dy[i]])
//                    {
//                        q.push({x + dx[i] , y + dy[i]});
//                        checks[x + dx[i]][y + dy[i]] = true;
//                    }
//            }
//        }
//    }
//public:
//    int numIslands(vector<vector<char>>& grid) {
//        int ans = 0;
//        int m = grid.size() , n = grid[0].size();
//        vector<vector<bool>> checks(m , vector<bool>(n));
//        for(int i = 0; i < m; i++) {
//            for(int j = 0; j < n; j++) {
//                if(grid[i][j] == '1' && !checks[i][j]) {
//                    ans++;
//                    bfs(grid , checks , i , j);
//                }
//            }
//        }
//        return ans;
//    }
//};
