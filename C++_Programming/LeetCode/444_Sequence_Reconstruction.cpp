class Solution {
public:
    bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
        if(org.size()==0 || seqs.empty()) return false;
        int sz = org.size();
        vector<int> indgr(sz+1);
        vector<unordered_set<int>> graph(sz+1);
        for(int i=0;i<seqs.size();i++)
        {
            build(indgr,graph,seqs[i]);    
        }
        vector<int> recon;
        queue<int> q;
        for(int i=1;i<=sz;i++)
        {
            if(indgr[i]==0) q.push(i);
        }        
        while(!q.empty())
        {
            if(q.size()!=1) return false;
            int node = q.front();
            recon.push_back(node);
            q.pop();
            
            for(auto it=graph[node].begin();it!=graph[node].end();++it)
            {
                int tmp = *it;
                indgr[tmp]--;
                if(indgr[tmp]==0) q.push(tmp);
            }   
        }
        if(recon.size()!=sz) return false;
        for(int i=0;i<sz;i++)
        {
            if(recon[i]==org[i]) continue;
            return false;
        }
        return true;
    }
    
    void build(vector<int>& indgr,vector<unordered_set<int>>& graph,
               vector<int>& edges)
    {
        int sz = edges.size();
        if(sz==1) return;
        for(int i=0;i<sz-1;i++)
        {
            int from = edges[i];
            int to   = edges[i+1];
            if(graph[from].count(to)==1) continue;
            indgr[to]++;
            graph[from].insert(to);
        }
        return;
    }
};