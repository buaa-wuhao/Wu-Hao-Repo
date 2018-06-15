
class Solution {
public:
    bool sequenceReconstruction(vector<int>& org, vector<vector<int>>& seqs) {
        if(org.size()==0 || seqs.empty()) return false;
        int sz = org.size();
        vector<int> indgr(sz+1,-1);
        vector<unordered_set<int>> graph(sz+1);
        for(int i=0;i<seqs.size();i++)
        {
            if(seqs[i].size()==1)
            {
                if(seqs[i][0]<1 || seqs[i][0]>org.size()) return false;
                if(indgr[seqs[i][0]]<0) indgr[seqs[i][0]] = 0;
                continue;
            }
            if(build(indgr,graph,seqs[i])) continue;
            return false;
        }
        vector<int> recon;
        queue<int> q;
        for(int i=1;i<=sz;i++)
        {
            if(indgr[i]==0 && !graph[i].empty()) 
            {
                q.push(i);
            }
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
    
    bool build(vector<int>& indgr,vector<unordered_set<int>>& graph,
               vector<int>& edges)
    {
        int sz = edges.size();
        int n  = indgr.size();
        for(int i=0;i<sz-1;i++)
        {
            int from = edges[i];
            int to   = edges[i+1];
            if(from<1||from>=n || to<1 || to>=n) return false;
            if(graph[from].count(to)==1) continue;
            if(indgr[from]<0) indgr[from] = 0;
            if(indgr[to]<0) indgr[to] = 0;
            indgr[to]++;
            graph[from].insert(to);
        }
        return true;
    }
};