//class Solution {
//public:
//    int lastStoneWeight(vector<int>& stones) {
//        // 很明显直接优先级队列
//        priority_queue<int> heap; // 默认大堆
//        for(auto val : stones) heap.push(val);
//        while(heap.size() > 1) {
//            int x = heap.top(); heap.pop();
//            int y = heap.top(); heap.pop();
//            if(x > y) heap.push(x - y);
//            else if(y > x) heap.push(y - x);
//        }
//        return heap.empty() ? 0 : heap.top();
//    }
//};
//
//class KthLargest {
//    int _k;
//    priority_queue<int , vector<int> , greater<int>> _heap;
//public:
//    KthLargest(int k, vector<int>& nums) 
//        :_k(k)
//    {
//        for(auto val : nums) {
//            _heap.push(val);
//            if(_heap.size() > k) _heap.pop();
//        }
//    }
//    
//    int add(int val) {
//        _heap.push(val);
//        if(_heap.size() > _k) _heap.pop();
//        return _heap.top();
//    }
//};
//
///**
// * Your KthLargest object will be instantiated and called as such:
// * KthLargest* obj = new KthLargest(k, nums);
// * int param_1 = obj->add(val);
// */
//
//class Solution {
//public:
//    struct compare{
//        bool operator()(const pair<string , int>& p1 , const pair<string , int>& p2) {
//            return (p1.second < p2.second) || (p1.second == p2.second && p1.first > p2.first);
//        }
//    };
//    vector<string> topKFrequent(vector<string>& words, int k) {
//        unordered_map<string , int> hash;
//        for(auto& word: words) hash[word]++;
//        priority_queue<pair<string , int> , vector<pair<string,int>> , compare> heap;
//        for(auto& kv : hash) heap.push(kv);
//        vector<string> ans;
//        while(k--) {
//            ans.push_back(heap.top().first);
//            heap.pop();
//        }
//        return ans;
//    }
//};
