/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
class Solution {
public:
    vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
        vector<Interval> res;
        if(intervals.empty()) 
        {
            res.push_back(newInterval);
            return res;
        }
        int sz = intervals.size();
        pair<long,long> merged = {newInterval.start,newInterval.end};
        
        for(int i=0;i < sz;i++)
        {
            if(intervals[i].end < merged.first) 
            {
                res.push_back(intervals[i]);
                continue;
            }
            if(merged.second < intervals[i].start)
            {
                res.push_back(Interval(merged.first,merged.second));
                merged.first = (long)INT_MAX + 1;
                //cout << merged.first << endl;
                i--;
                continue;
            }
            merged.first = min(merged.first,(long)intervals[i].start);
            merged.second   = max(merged.second,(long)intervals[i].end);
        }
        
        if(merged.first!= (long)INT_MAX+1) res.push_back(Interval(merged.first,merged.second));
        return res;
    }
};