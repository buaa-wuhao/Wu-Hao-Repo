class Solution {
public:
    int countPrimes(int n) {
        if(n<=2) return 0;        
        vector<long> primes{2};
        
        for(long i=3;i<n;i++)
        {
            int j;
            for( j=0;j<primes.size() && primes[j]*primes[j] < i;j++)
            {
                if(i%primes[j]==0) break;
            }
            if(j==primes.size() || i%primes[j]!=0)
            {
                primes.push_back(i);
            }
        }
        return primes.size();
    }
};

class Solution {
public:
    int countPrimes(int n) {
        if(n<=2) return 0;
        vector<int> primes(n+1,1);
        primes[2] = 1;
        for(int i=2;i*i<n;i++)
        {
            if(primes[i]==0) continue;
            
            for(int j=i;j*i<n;j++)
            {
                primes[j*i]=0;
            }
        }
        int cnt = 0;
        for(int i=2;i<n;i++)
        {
            if(primes[i]) cnt++;
        }
        return cnt;
    }
};