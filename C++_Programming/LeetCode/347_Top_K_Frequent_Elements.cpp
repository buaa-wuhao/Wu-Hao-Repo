class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
      
        unordered_map<int,int> f_map;
        
        for(int i=0;i < nums.size();i++)
        {
            f_map[nums[i]]++;
        }
        
        priority_queue<pair<int,int>,vector<pair<int,int>>,mycomp> topk;
        
        for(auto& x: f_map)
        {
            if(topk.size() < k)
            {
                topk.push({x.first,x.second});
                continue;
            }
            if(topk.top().second >= x.second) continue;
            
            topk.pop();
            topk.push({x.first,x.second});
        }
        
        vector<int> result;
        
        while(!topk.empty())
        {
            result.insert(result.begin(),topk.top().first);
            topk.pop();
        }
        return result;
    }
};