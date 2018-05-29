class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.empty()) return 0;
        int sz = nums.size();
        vector<int> profit(sz,0);
        profit[0] = nums[0];
        profit[1] = max(nums[0],nums[1]);
        
        for(int i=2;i<sz;i++)
        {
            profit[i] = max(profit[i-2]+nums[i],profit[i-1]);
        }
        
        return profit[sz-1];
    }
};