
class Solution {
public:
    vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
        vector<pair<int,int>> hi_map;
        
        for(int i=0;i<buildings.size();i++)
        {
            hi_map.push_back({buildings[i][0],-buildings[i][2]});
            hi_map.push_back({buildings[i][1], buildings[i][2]});
        }
        
        sort(hi_map.begin(),hi_map.end());
        
        multiset<int> max_height;
        max_height.insert(0);
        
        vector<pair<int,int>> result;
        int prev = 0;
        for(auto& it: hi_map)
        {
           // cout<<it.first<<" "<<it.second<<endl;
            if(it.second < 0)
            {
                max_height.insert(-it.second);
            }
            else
            {
                max_height.erase(max_height.find(it.second));
            }
            
            int cur = *(max_height.rbegin());
            
            if(cur != prev)
            {
                result.push_back({it.first,cur});
                prev = cur;
            }
            
        }
        return result;
    }
};