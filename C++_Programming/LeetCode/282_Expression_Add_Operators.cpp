class Solution {
public:
    vector<string> addOperators(string num, int target) {
        if(num.empty()) return {};
        vector<string> paths;
        string path;
        dfs(num,0,0,0,target,path,paths);
        return paths;
    }
    
    void dfs(string& num, int start, long pre_val,
             long pre_num, int tgt, string path, vector<string>& paths)
    {
        if(start==num.size()) 
        {
            if(tgt==pre_val) paths.push_back(path);            
        }
        
        for(int i=start;i<num.size();i++)
        {
            string str = num.substr(start,i-start+1);
            long cur = stol(str);
            if(start==0)
            {
                dfs(num,i+1,pre_val+cur,cur,tgt,str,paths);
            }
            else
            {
                dfs(num,i+1,pre_val+cur,cur,tgt,path+"+"+str,paths);
                dfs(num,i+1,pre_val-cur,-cur,tgt,path+"-"+str,paths);
                dfs(num,i+1,pre_val-pre_num+pre_num*cur,pre_num*cur,tgt,path+"*"+str,paths);
            }
            
            if(num[start]=='0') break;
        }
        
        return;
    }
};