class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if(grid.empty() || grid[0].empty()) return 0;
        int rows = grid.size();
        int cols = grid[0].size();
        int cnt = 0;
        
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(grid[i][j]=='0') continue;
                cnt++;
                bfs(grid,i,j);
            }
        }
        
        return cnt;
    }
    
    void bfs(vector<vector<char>>& grid,int x,int y)
    {
        vector<pair<int,int>> mv = {{1,0},{-1,0},{0,-1},{0,1}};
        queue<pair<int,int>> Q;
        Q.push({x,y});
        int m = grid.size();
        int n = grid[0].size();
        grid[x][y] = '0';
        while(!Q.empty())
        {
            pair<int,int> pos = Q.front();
            Q.pop();
            //grid[pos.first][pos.second] = 0;
            for(int i=0;i<mv.size();i++)
            {
                int new_x = pos.first + mv[i].first;
                int new_y = pos.second + mv[i].second;
                if(new_x<0 || new_x>=m || new_y<0 || new_y>=n) continue;
                if(grid[new_x][new_y]=='0') continue;
                Q.push({new_x,new_y});
                //Must set zero here, instead of "grid[pos.first][pos.second] = 0;" above
                //Set zero here, to prevent same position being added into queue multiple times
                //which causes memory over limit issue.
                grid[new_x][new_y] = '0';
                
            }
        }
        return;
    }
};