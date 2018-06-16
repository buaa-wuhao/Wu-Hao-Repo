class Trie{
public:
    vector<Trie*> a2z;
    bool is_end;
    Trie():is_end(false),a2z(26,NULL) {};
    
    void add(string word)
    {
        Trie* node = this;
        for(int i=0;i<word.size();i++)
        {
            int idx = word[i]-'a';
            if(node->a2z[idx]==NULL) node->a2z[idx] = new Trie();
            node = node->a2z[idx];
        }
        node->is_end = true;
        return;
    };
};

class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        if(words.empty()) return {};
        Trie* ptr = new Trie();
        
        for(int i=0;i<words.size();i++)
        {
            ptr->add(words[i]);
        }
        int m = board.size();
        int n = board[0].size();        
        unordered_set<string> ans;
        for(int i=0;i<board.size();i++)
        {
            for(int j=0;j<board[0].size();j++)
            {
                vector<vector<bool>> visited(m,vector<bool>(n,false));
                string word;
                dfs(board,i,j,ans,visited,word,ptr);
            }
        }
        release(ptr);
        vector<string> res;
        for(auto& it: ans)
        {
            res.push_back(it);
        }
        return res;
        
    }
    
    void dfs(vector<vector<char>>&board, int x,int y,
             unordered_set<string>& ans,vector<vector<bool>>& visited,
             string word,Trie* ptr)
    {
        int m = board.size();
        int n = board[0].size();
        char c = board[x][y];              
        if(ptr->a2z[c-'a']==NULL) return;
        visited[x][y] = true;
        word.push_back(c);  
        if(ptr->a2z[c-'a']->is_end) ans.insert(word);
        
        vector<pair<int,int>> mv = {{0,1},{0,-1},{1,0},{-1,0}};                
        for(int jj=0;jj<mv.size();jj++)
        {
            int new_x = x + mv[jj].first;
            int new_y = y + mv[jj].second;
            if(new_x<0 || new_y <0 || new_x>=m || new_y>= n) continue;
            if(visited[new_x][new_y]) continue;          
            dfs(board,new_x,new_y,ans,visited,word,ptr->a2z[c-'a']);
            
        }
        visited[x][y] = false;
        return;
    }
    
    void release(Trie* ptr)
    {
        if(ptr==NULL) return;
        for(int i=0;i<26;i++)
        {
            release(ptr->a2z[i]);
        }
        delete ptr;
        return;
    }
};

