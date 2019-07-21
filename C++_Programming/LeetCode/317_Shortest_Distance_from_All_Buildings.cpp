class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
		if(grid.empty() || grid[0].empty()) return 0;
		queue<pair<int,int>> q;
		int m = grid.size();
		int n = grid[0].size();
		vector<vector<int>> dist(m,vector<int>(n,0));
		vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};		
		int tgt = 0,min_dist=INT_MAX,lvl =0;
		for(int i=0;i<m;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(grid[i][j]!=1) continue;
				q.push({i,j});
				dist[i][j] = 0;
				min_dist = INT_MAX;
                lvl = 0;
				while(!q.empty())
				{
					int sz = q.size();
                    lvl++;
                    for(int j=0;j<sz;j++)
                    {
                        pair<int,int> cur = q.front();
					    q.pop();
					    for(int i=0;i<mv.size();i++)
					    {
		                    int new_x = cur.first + mv[i].first;
		                    int new_y = cur.second + mv[i].second;
		                    if(new_x<0 || new_y<0 || new_x >= m || new_y >= n) continue;
		                    if(grid[new_x][new_y]!= tgt) continue;	
						    dist[new_x][new_y] += lvl;
						    min_dist = min(min_dist,dist[new_x][new_y]);
						    grid[new_x][new_y]--;
						    q.push({new_x,new_y});
					    }
                    }
				}
				tgt--;				
			}
		}
		return min_dist==INT_MAX ? -1:min_dist ;
    }
};
