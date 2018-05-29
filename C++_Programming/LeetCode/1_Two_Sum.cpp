class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> h_tbl;
        
        for(int i=0;i < nums.size();i++)
        {
            if(h_tbl.find(target-nums[i])==h_tbl.end())
            {
                h_tbl[nums[i]] = i;
                continue;
            }
            
            return {h_tbl[target - nums[i]],i};
        }
    }
};
