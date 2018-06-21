class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        if(nums.empty()) return false;
        unordered_map<int,int> _hash;
        for(int i=0;i<nums.size();i++)
        {
            if(_hash.count(nums[i])==0)
            {
                _hash[nums[i]] = i;
            }
            else
            {
                if(i-_hash[nums[i]]<=k) return true;
                _hash[nums[i]] = i;
            }
        }
        return false;
    }
};