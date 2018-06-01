class UF{
private:
  int cnt;
  vector<int>  id;
  vector<int>  sz;
public:
    UF(int n);
    bool connected(int p,int q);
    int  count();
    void Union(int p,int q);
    int find(int p);
};

UF::UF(int n)
{
    cnt = n;
    for(int i=0;i<n;i++)
    {
        id.push_back(i);
        sz.push_back(1);
    }
}

int UF::find(int p)
{
    while(id[p]!=p) p = id[p];
    return p;
}

bool UF::connected(int p,int q)
{
    return (find(p)==find(q));
}

int UF::count()
{
    return cnt;
}

void UF::Union(int p,int q)
{   
    p = find(p);
    q = find(q);
    if(p==q) return ;// 第一次写，忘记了这行。
    if(sz[p]<=sz[q])
    {
        id[p] = q; 
        sz[q]+=sz[p];
    }
    else
    {
        id[q] = p;
        sz[p]+=sz[q];
    }
    cnt--;
    return;
}

class Solution {
public:
    vector<int> numIslands2(int m, int n, vector<pair<int, int>>& positions) {
        if(m==0 || n==0 || positions.empty()) return {};
        
        int sz = positions.size();
        UF uf(sz);
        unordered_map<int,int> _hash;
        _hash[positions[0].first*n+positions[0].second] = 0;
        
        vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};
        vector<int> ans(1,1);
        int nums = 1;
        
        for(int i=1;i< sz ;i++)
        {
            nums++;
            for(int j=0;j<mv.size();j++)
            {
                int new_x = positions[i].first  + mv[j].first;
                int new_y = positions[i].second + mv[j].second;
                if(new_x<0 || new_x>=m || new_y<0 || new_y>=n) continue;
                
                int ind = new_x*n+new_y;
                
                if(_hash.count(ind) && !uf.connected(_hash[ind],i))
                {
                    uf.Union(i,_hash[ind]);
                    nums--;
                }
            }
            _hash[positions[i].first*n+positions[i].second] = i;
            ans.push_back(nums);
        }
        return ans;
    }
};