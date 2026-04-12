//class Solution {
//public:
//    int maxProfit(vector<int>& prices) {
//        // f[i][j]表示第i结束后，完成了j笔交易，持有股票，此时的最大利润
//        // g[i][j]表示第i结束后，完成了j笔交易，不持有股票，此时的最大利润
//        int n = prices.size();
//        const int INFINITE = 0x3f3f3f3f;
//        vector<vector<int>> f(n , vector<int>(3 , -INFINITE)) , g(n , vector<int>(3 , -INFINITE));
//        f[0][0] = -prices[0];
//        g[0][0] = 0;
//        for(int i = 1; i < n; i++) {
//            for(int j = 0; j < 3; j++) {
//                f[i][j] = max(f[i - 1][j] , g[i - 1][j] - prices[i]);
//                g[i][j] = g[i - 1][j];
//                if(j >= 1) g[i][j] = max(g[i][j] , f[i - 1][j - 1] + prices[i]);
//            }
//        }
//        // std::cout << f[1][0] << " " << f[1][1] << " " << f[1][2] << std::endl;
//        // std::cout << g[1][0] << " " << g[1][1] << " " << g[1][2] << std::endl;
//        return max(g[n - 1][0] , max(g[n - 1][1] , g[n - 1][2]));
//    }
//};
//
//class Solution {
//public:
//    int maxProfit(int k, vector<int>& prices) {
//        // f[i][j]表示第i结束后，完成了j笔交易，持有股票，此时的最大利润
//        // g[i][j]表示第i结束后，完成了j笔交易，不持有股票，此时的最大利润
//        int n = prices.size();
//        const int INFINITE = 0x3f3f3f3f;
//        vector<vector<int>> f(n , vector<int>(k + 1 , -INFINITE)) , g(n , vector<int>(k + 1 , -INFINITE));
//        f[0][0] = -prices[0];
//        g[0][0] = 0;
//        for(int i = 1; i < n; i++) {
//            for(int j = 0; j < k + 1; j++) {
//                f[i][j] = max(f[i - 1][j] , g[i - 1][j] - prices[i]);
//                g[i][j] = g[i - 1][j];
//                if(j >= 1) g[i][j] = max(g[i][j] , f[i - 1][j - 1] + prices[i]);
//            }
//        }
//        int ans = 0;
//        for(int j = 0; j < k + 1; j++) ans = max(ans , g[n - 1][j]);
//        return ans;
//    }
//};
