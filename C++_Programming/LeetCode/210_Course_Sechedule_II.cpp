class Solution {
public:
    vector<int> findOrder(int numCourses, vector<pair<int, int>>& prerequisites) {
        int n = numCourses;
        if(n==0) return {};
        vector<int> indegree(n);
        vector<vector<int>> graph(n);
        int sz = prerequisites.size();
        
        for(int i=0;i<sz;i++)
        {
            int from = prerequisites[i].second;
            int to   = prerequisites[i].first;
            indegree[to]++;
            graph[from].push_back(to);
        }
        queue<int> q;
        for(int i=0;i<n;i++)
        {
            if(indegree[i]==0) q.push(i);
        }
        if(q.empty()) return {};
        vector<int> odr;        
        while(!q.empty())
        {
            int cur = q.front();
            q.pop();
            odr.push_back(cur);
            for(int i=0;i<graph[cur].size();i++)
            {
                int tmp = graph[cur][i];
                indegree[tmp]--;
                if(indegree[tmp]==0) q.push(tmp);
            }
        }
        if(odr.size()==n) return odr;
        return {};
    }
};