//class Solution {
//public:
//    int maxProfit(vector<int>& prices, int fee) {
//        // f[i]表示第i结束后，持有股票，此时的最大利润
//        // g[i]表示第i结束后，不持有股票，此时的最大利润
//        int n = prices.size();
//        vector<int> f(n) , g(n);
//        f[0] = -prices[0];
//        for(int i = 1; i < n; i++) {
//            f[i] = max(f[i - 1] , g[i - 1] - prices[i]);
//            g[i] = max(g[i - 1] , f[i - 1] + prices[i] - fee);
//        }
//        return max(f[n - 1] , g[n - 1]);
//    }
//};
