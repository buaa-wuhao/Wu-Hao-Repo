class Solution {
public:
    int nthUglyNumber(int n) {
        if(n==1) return 1;
        
        vector<int> seq = {1};
        int t2 = 0;
        int t3 = 0;
        int t5 = 0;
        int idx = 1;
        
        while(idx < n)
        {
            int tmp = min(seq[t2]*2,min(seq[t3]*3,seq[t5]*5));
            seq.push_back(tmp);
            
            if(tmp == seq[t2]*2) t2++;
            if(tmp == seq[t3]*3) t3++;
            if(tmp == seq[t5]*5) t5++;
            idx++;
        }
        
        return seq.back();
    }
};

class Solution {
public:
    int nthUglyNumber(int n) {
        if(n==1) return 1;
        priority_queue<long,vector<long>,greater<long>> pq;
        unordered_set<long>  sets;
        vector<long> multi {2,3,5};
        pq.push(1);
        sets.insert(1);
        for(int i=0;i<n;i++)
        {
            long top = pq.top();
            if(i==n-1) return top;
            pq.pop();
            for(int j=0;j<multi.size();j++)
            {
                long tmp = multi[j]*top;
                if(sets.count(tmp)) continue;
                sets.insert(tmp);
                pq.push(tmp);
            }
        }
    }
};