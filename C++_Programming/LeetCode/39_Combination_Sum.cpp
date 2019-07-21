class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        int sz = candidates.size();
        vector<vector<int>> A(sz,vector<int>(target+1,0));
        for(int i=0;i<sz;i++)
        {
            A[i][0] = 1;
        }
        
        for(int i=0;i<sz;i++)
        {
            for(int j=1;j<=target;j++)
            {
                A[i][j] = ((i>=1)?A[i-1][j]:0) +
                          (j>=candidates[i]?A[i][j-candidates[i]]:0);
                //cout<<A[i][j]<<" ";
            }
            //cout<<endl;
        }
        
        //cout << A[sz-1][target] << endl;
        
        vector<vector<int>> paths;
        vector<int> path;
        if(A[sz-1][target]) search(sz-1,target,path,paths,A,candidates);
        
        return paths;
    }
    
void  search(int x,int y, vector<int> path,
             vector<vector<int>>& paths,
             vector<vector<int>>& A,
             vector<int>& cand)
      {
           
            if(y==0)
            {
                paths.push_back(path);
                return;
            }   
    
            if(x==0 || A[x][y]!=A[x-1][y])
            {
                path.insert(path.begin(),cand[x]);
                search(x,y-cand[x],path,paths,A,cand);
                path.erase(path.begin());
            }

            if(x>0 && A[x-1][y])
            {
                search(x-1,y,path,paths,A,cand);
            }
            
            return;
      }
};