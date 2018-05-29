class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.empty()) return 0;
        int sz = nums.size();
        if(sz==1) return nums[0];
        if(sz==2) return max(nums[0],nums[1]);
        
        return max(rb(nums,0,sz-2),rb(nums,1,sz-1));
    }
    
    int rb(vector<int>& nums,int start,int end)
    {
        if(start==end)  return nums[start];
        if(start+1==end)return max(nums[start],nums[end]);
        
        vector<int> max_p(end-start+1);
        max_p[0] = nums[start];
        max_p[1] = max(nums[start+1],nums[start]);
        
        for(int i=2;i<max_p.size();i++)
        {
            max_p[i] = max(max_p[i-2]+nums[start+i],max_p[i-1]);    
        }
        
        return max_p.back();
    }
};