
class Solution {
public:
    bool validTree(int n, vector<pair<int, int>>& edges) {
         if(n==0 || edges.size()<n-1) return false;
         vector<int>  cnt(n);
         vector<unordered_set<int>>  graph(n);
         
         for(int i=0;i<edges.size();i++)
         {
             int first = edges[i].first;
             int second= edges[i].second;
             cnt[first]++;
             cnt[second]++;
             graph[first].insert(second);
             graph[second].insert(first);
         }
         queue<int> q;
         for(int i=0;i<n;i++)
         {
             if(cnt[i]==1 || cnt[i]==0 ) q.push(i);
         }
         if(q.empty()) return false;
         int count = 0;
         while(!q.empty())
         {
             int node = q.front();
             q.pop();
             count++;
             if(cnt[node]==0) continue;
             cnt[node]--;
             int peer = *(graph[node].begin());
             cnt[peer]--;
             graph[peer].erase(node);
             if(cnt[peer]==1) q.push(peer); 
         }
         if(count==n) return true;
         return false;
           
    }
};