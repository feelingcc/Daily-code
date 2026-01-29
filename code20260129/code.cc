//class Solution {
//public:
//    void sortColors(vector<int>& nums) {
//        // 三指针操作
//        // i指针遍历数组、left指向0元素要放的位置、right指向2元素要放到位置
//        for(int i = 0 , left = 0 , right = nums.size() - 1; i <= right; i++) {
//            if(nums[i] == 0) swap(nums[left++] , nums[i]);
//            else if(nums[i] == 2) swap(nums[right--] , nums[i--]);
//        }   
//    }
//};
//
//
//class Solution {
//public:
//    void partation(vector<int>& nums , int begin , int end) {
//        if(begin >= end) return;
//        int key = nums[rand() % (end - begin + 1) + begin];
//        int left = begin , right = end;
//        for(int i = begin; i <= right; i++) {
//            if(nums[i] < key) swap(nums[i] , nums[left++]);
//            else if(nums[i] > key) swap(nums[i--] , nums[right--]);
//        }
//        partation(nums , begin , left - 1);
//        partation(nums , right + 1 , end);
//    }
//    vector<int> sortArray(vector<int>& nums) {
//        srand(time(nullptr));
//        partation(nums , 0 , nums.size() - 1);
//        return nums;
//    }
//};
