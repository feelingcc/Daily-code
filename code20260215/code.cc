class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string , vector<string>> hash;
        for(auto& str : strs) {
            string temp = str;
            sort(temp.begin() , temp.end());
            hash[temp].push_back(str);
        }
        vector<vector<string>> ans;
        for(auto& kv : hash) {
            ans.push_back(kv.second);
        }
        return ans;
    }
};

class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int,int> hash; // [val , index]
        for(int i = 0; i < nums.size(); i++) {
            if(hash.count(nums[i]) && abs(hash[nums[i]] - i) <= k) return true;
            hash[nums[i]] = i;
        }
        return false;
    }
};


