class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<pair<int, int>>& edges) {
         if(n==0 ) return {};
         vector<int> cnt(n);
         vector<unordered_set<int>> graph(n);
         
         for(int i=0;i<edges.size();i++)
         {
            int first  = edges[i].first;
            int second = edges[i].second;
            cnt[first]++;
            cnt[second]++;
            graph[first].insert(second); 
            graph[second].insert(first);
         }
        
        queue<int> q;
        for(int i=0;i<n;i++)
        {
            if(cnt[i]==1 || cnt[i]==0) {q.push(i);}
        }
        if(q.empty()) return {};
        vector<int> mht;
        
        while(!q.empty())
        {
            int sz = q.size();
            if(!mht.empty()) mht.clear();
            for(int i=0;i<sz;i++)
            {
                int node = q.front();
                mht.push_back(node);
                q.pop();
                if(cnt[node]==0) continue;
                cnt[node]--;
                int peer = *(graph[node].begin());
                cnt[peer]--;
                graph[peer].erase(node);
                if(cnt[peer]==1) q.push(peer);
            }
        }
        return mht;
        
    }
};