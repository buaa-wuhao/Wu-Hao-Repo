//DFS
class Trie {
private:
    Trie* a2z[26];
    bool  f_end;
public:
    /** Initialize your data structure here. */
    Trie() {
        f_end = false;
        for(int i=0;i<26;i++)
        {
            a2z[i] = NULL;
        }
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
         if(word.empty()) 
         {
            f_end = true;
            return;
         }
         Trie* ptr = a2z[word[0]-'a'];
         if(ptr==NULL)
         {
             ptr = new Trie();
             a2z[word[0]-'a'] = ptr;
         }
         string str = word.substr(1);
         ptr->insert(str);
         return; 
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        if(word.empty()) 
        {
            return (f_end==true?true:false);
        }
        if(a2z[word[0]-'a']==NULL) return false;
        Trie* ptr = a2z[word[0]-'a'];
        string str = word.substr(1);
        return ptr->search(str);
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        if(prefix.empty()) 
        {return true;}
        if(a2z[prefix[0]-'a']==NULL) return false;
        Trie* ptr = a2z[prefix[0]-'a'];
        string str = prefix.substr(1);
        return ptr->startsWith(str);
    }
};




//Iterative
class Trie {
    vector<Trie*> link;
    bool IsEnd;

    public:
    /** Initialize your data structure here. */
    Trie():IsEnd(false),link(26,NULL) {
        
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) 
    {
        Trie* node=this;
        
        for(int i=0;i < word.size();i++)
        {
            if(node->link[word[i]-'a']==NULL)
            {
                Trie* new_node = new Trie();
                node->link[word[i]-'a'] = new_node;
                node = new_node;
            }
            else
            {
                node = node->link[word[i]-'a']; 
            }
        }
        node->IsEnd = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) 
    {
        Trie* node = this;
        for(int i=0;i < word.size();i++)
        {
            if(node->link[word[i]-'a']==NULL)
            {
                return false;
            }
            node = node->link[word[i]-'a'];
        }
        if(node->IsEnd==true) return true;
        return false;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) 
    {
        Trie* node = this;
        for(int i=0;i < prefix.size();i++)
        {
            if(node->link[prefix[i]-'a']==NULL) return false;
            node = node->link[prefix[i]-'a'];
        }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie obj = new Trie();
 * obj.insert(word);
 * bool param_2 = obj.search(word);
 * bool param_3 = obj.startsWith(prefix);
 */