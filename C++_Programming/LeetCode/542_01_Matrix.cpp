class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) {
        if(matrix.empty() || matrix[0].empty()) return {};       
        int m = matrix.size();       
        int n = matrix[0].size();     

        vector<vector<int>> output(m,vector<int>(n,INT_MAX));        
        /*
        vector<vector<int>> dist(m,vector<int>(n,INT_MAX)); 
        bfs(1,0,matrix,dist);
        combine(output,dist);*/
        
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(matrix[i][j]!=0) continue;
                vector<vector<int>> dist(m,vector<int>(n,INT_MAX)); 
                bfs(i,j,matrix,dist);
                combine(output,dist);
            }
        }
 
        return output;
    }
    void combine(vector<vector<int>>& output,
                 vector<vector<int>>& dist)
    {
        int m = output.size();       
        int n = output[0].size();   
        
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                //cout<<dist[i][j]<<" ";
                output[i][j] = min(output[i][j],dist[i][j]);        
            }
            //cout<<endl;
        }
        return;
    }
    void bfs(int x,int y, vector<vector<int>>& matrix,
             vector<vector<int>>& output)
    {
        vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};
        int m = matrix.size();
        int n = matrix[0].size();
        int lvl = 0;
        queue<pair<int,int>> Q;
        Q.push({x,y});
        output[x][y] = 0;
        while(!Q.empty())
        {
            lvl++;
            int sz = Q.size();
            for(int i=0;i<sz;i++)
            {
                pair<int,int> cur = Q.front();
                Q.pop();
                for(int j=0;j<mv.size();j++)
                {
                    int new_x = cur.first + mv[j].first;
                    int new_y = cur.second + mv[j].second;
                    if(new_x<0 || new_y<0 || new_x>=m || new_y>=n) continue;                    
                    if(matrix[new_x][new_y]==0) continue;
                    if(output[new_x][new_y]!=INT_MAX) continue;
                    output[new_x][new_y] = lvl;
                    //cout<< new_x <<" " << new_y << " " << lvl << endl;
                    Q.push({new_x,new_y});
                } 
            }
        }
        return;
    }
};