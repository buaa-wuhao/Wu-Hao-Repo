class Solution {
public:
    bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
        int n = numCourses;
        if(n==0 || prerequisites.empty()) return true;
        vector<int>           indegree(n,0);
        vector<vector<int>>   graph(n);
        
        for(int i=0;i<prerequisites.size();i++)
        {
            int to       = prerequisites[i].first;
            int from     = prerequisites[i].second;
            indegree[to]++;
            graph[from].push_back(to);
        }
        queue<int> q;
        for(int i=0;i<indegree.size();i++)
        {
            if(indegree[i]==0) q.push(i);
        }
        int sz = q.size();
        if(sz==0) return false;
        int cnt = 0;
        while(!q.empty())
        {
            int i = q.front();
            q.pop();
            for(int j=0;j<graph[i].size();j++)
            {
                int node = graph[i][j];
                indegree[node]--;
                if(indegree[node]==0) 
                {
                    q.push(node);
                    cnt++;
                }
            }
        }
        if(cnt+sz==numCourses) return true;
        return false;
        
    }
};