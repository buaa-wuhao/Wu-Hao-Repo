class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        if(grid.empty() || grid[0].empty()) return 0;
        vector<pair<int,int>> Q;
        int rows = grid.size();
        int cols = grid[0].size();     
        vector<vector<int>> g_dists(rows,vector<int>(cols,0));          
        
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(grid[i][j]==1) 
                {
                    Q.push_back({i,j});
                }
            }
        }
    
    vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}}; 
       
	for(int i=0;i<Q.size();i++) 
	{
        queue<pair<int,int>> q;
	    q.push(Q[i]);
	    
        int dist = 1;
        vector<vector<int>> dists(rows,vector<int>(cols,INT_MAX));  	    
	    
        while(!q.empty()) 
	    {
	      int sz = q.size();	      
          for(int j=0;j<sz;j++)
	      {
	     	 pair<int,int> pos = q.front();
		     q.pop();		
		     
             for(int k=0;k<mv.size();k++)
		     {
		         int x = pos.first + mv[k].first;
	             int y = pos.second + mv[k].second;	             
                 if(x<0 || y<0 || x >= rows || y>=cols) continue;
		         if(grid[x][y]==1 || grid[x][y]==2) continue;
		         if(dists[x][y] != INT_MAX) continue;	             
                 
                 q.push({x,y});
                 dists[x][y] = dist;     
		     }
	      }
	      dist++;
	    } 		    
        
        for(int ii=0;ii<rows;ii++)
	    {
            for(int jj=0;jj<cols;jj++)
	        {
                //cout<<dists[ii][jj] << " ";
                if(g_dists[ii][jj]!=INT_MAX && dists[ii][jj]!=INT_MAX)
                 g_dists[ii][jj] += dists[ii][jj];
                 else
                 g_dists[ii][jj] = INT_MAX;    
	        }
           // cout << endl;
	    }
   	  }
        
	int min_dist = INT_MAX;
	for(int i=0;i<rows;i++)
	{
	     for(int j=0;j<cols;j++)
	     {
             //cout<<g_dists[i][j] << " ";
             min_dist = min(min_dist,g_dists[i][j]);
	     }
        //cout << endl;
	}

	if(min_dist==INT_MAX) return -1;
	return min_dist;
                 	
    }
};
