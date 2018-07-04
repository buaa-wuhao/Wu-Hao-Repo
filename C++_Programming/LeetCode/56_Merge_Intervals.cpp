/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
bool comp(Interval& l,Interval& r) 
{
    if(l.start < r.start) return true;
    return false;
}
class Solution {
public:
    vector<Interval> merge(vector<Interval>& intervals) {
        if(intervals.empty()) return {};
        
        sort(intervals.begin(),intervals.end(),comp);
        vector<Interval> res;
        
        Interval intvl = intervals[0];
        for(int i=0;i<intervals.size()-1;i++)
        {
            //cout<<intervals[i].start << " " << intervals[i].end << endl;
            if(intvl.end < intervals[i+1].start)
            {
                res.push_back(intvl);
                intvl = intervals[i+1];
                continue;
            }
            
            if(intvl.end <= intervals[i+1].end)
            {
                intvl.end = intervals[i+1].end; 
            }
        }
        res.push_back(intvl);
        
        return res;
    }
};