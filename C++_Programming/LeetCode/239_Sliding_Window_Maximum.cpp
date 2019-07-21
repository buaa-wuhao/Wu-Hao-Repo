class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        if(nums.empty()) return {};
        
        deque<int> Q;
        vector<int> msw;
        
        for(int i=0;i<k;i++)
        {
            while(!Q.empty() && nums[i]>nums[Q.back()])
            {
                Q.pop_back();
            }            
            Q.push_back(i);           
        }
        
        for(int i=k;i<=nums.size();i++)
        {
            msw.push_back(nums[Q.front()]);
            
            if(i-Q.front()==k)
            {
                Q.pop_front();
            }
            
            while(!Q.empty() && nums[i]>nums[Q.back()])
            {
                Q.pop_back();
            }
            
            Q.push_back(i);
        }
        
        return msw;
    }
};