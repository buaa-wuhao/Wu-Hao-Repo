class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if(board.empty() || board[0].empty()) return;
        int rows = board.size();
        int cols = board[0].size();
        vector<vector<bool>> flag(rows,vector<bool>(cols,false));
        queue<pair<int,int>> Q;
        
        for(int i=0;i<cols;i++)
        {
            if(board[0][i]=='O')      
            {
                Q.push({0,i});
                flag[0][i] = true;
            }
            if(board[rows-1][i]=='O') 
            {
                Q.push({rows-1,i});
                flag[rows-1][i] = true;
            }
        }
        for(int i=1;i<rows-1;i++)
        {
            if(board[i][0]=='O')
            {
                Q.push({i,0});
                flag[i][0] = true;
            }
            if(board[i][cols-1]=='O')
            {
                Q.push({i,cols-1});
                flag[i][cols-1] = true;
            }
        }
        vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};
        while(!Q.empty())
        {
            pair<int,int> pos = Q.front();
            Q.pop();
            for(int i=0;i<mv.size();i++)
            {
                int x = pos.first + mv[i].first;
                int y = pos.second + mv[i].second;
                if(x<0 || y<0 || x>= rows || y>=cols) continue;
                if(board[x][y]!='O') continue;
                if(flag[x][y]==true) continue;
                Q.push({x,y});
                flag[x][y] = true;
            }
        }
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(!flag[i][j] && board[i][j]=='O') board[i][j]='X';
            }
        }
        return;
    }
};


class UF
{
  private:
    vector<int> id;
    vector<int> weight;
    int cnt;
  public:
    UF(int n)
    {
        id.resize(n,0);
        weight.resize(n,0);
        cnt = n;
        for(int i=0;i<n;i++)
        {
            id[i] = i;
            weight[i]=1;
        }
    }
    int find(int i)
    {
        while(id[i]!=i) i = id[i];
        return i;
    }
    bool connected(int i,int j)
    {
        return (find(i)==find(j));
    }
    void _union(int i,int j)
    {
        int i_f = find(i);
        int j_f = find(j);
        if(weight[i_f] > weight[j_f])
        {
            id[j_f]=i_f;
            weight[i_f] += weight[j_f];
        }
        else
        {
            id[i_f] = j_f;
            weight[j_f]+= weight[i_f];
        }
        cnt--;
        return;
    }
};

class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if(board.empty() || board[0].empty()) return ;
        int m = board.size();
        int n = board[0].size();
        
        UF uf(m*n+1);
        vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};
        
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(board[i][j]=='X') continue;
                
                if(board[i][j]=='O' && (i==0 || i==m-1 || j==0 || j==n-1))
                {
                   // cout<<i<<" "<<j<<endl;
                    uf._union(i*n+j,m*n);
                    continue;
                }
                
                for(int k=0;k<mv.size();k++)
                {
                    int new_x = i + mv[k].first;
                    int new_y = j + mv[k].second;
                    if(new_x<0 || new_y<0 || new_x >=m || new_y>=n) continue;
                    if(board[new_x][new_y]=='X') continue;
                    if(uf.connected(i*n+j,new_x*n+new_y)==true) continue;
                    uf._union(i*n+j,new_x*n+new_y);
                }
            }
        }
        
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(board[i][j]=='O' && !uf.connected(i*n+j,m*n)) board[i][j]='X';
            }
        }
        return;
    }
};