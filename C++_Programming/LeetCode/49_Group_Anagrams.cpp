class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                              41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 
                              83, 89, 97, 101, 103};
        unordered_map<int,int>  _hash;
        vector<vector<string>>  res;
        
        for(auto& it: strs)
        {
            int key = 1;
            for(int i=0;i<it.size();i++)
            {
                key *= primes[it[i]-'a'];
            }
            
            if(_hash.count(key))
            {
                res[_hash[key]].push_back(it);
            }
            else
            {
                _hash[key] = res.size();
                vector<string> vs {it};
                res.push_back(vs);
            }
        }
        return res;
    }
};