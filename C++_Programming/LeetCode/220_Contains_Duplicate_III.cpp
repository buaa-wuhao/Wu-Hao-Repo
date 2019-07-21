class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
        if(nums.empty() || k<1 || t<0) return false;
        multiset<int> ms;
        
        for(int i=0;i<nums.size();i++)
        {             
            auto it_low = ms.lower_bound(nums[i]);
            auto it_up  = ms.upper_bound(nums[i]);
            
            if(it_low!=ms.end() && *it_low==nums[i]) 
            {
                cout<<*it_low << " " << i << endl;
                return true;}
            
            
            if(it_low!=ms.begin() && (*(--it_low)+t)>=nums[i]       ||
                it_up!=ms.end() && (nums[i]+t)>=*it_up )
            {
                cout<<"return"<<endl;
                return true;
            }
                           
            ms.insert(nums[i]);  
            if(i>=k) ms.erase(nums[i-k]);
        }
        return false;
    }
};