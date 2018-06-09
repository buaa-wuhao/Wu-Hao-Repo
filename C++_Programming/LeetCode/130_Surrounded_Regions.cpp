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