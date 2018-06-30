class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        	
        	int sz1 = nums1.size();
        	int sz2 = nums2.size();
        	if((sz1+sz2) % 2 == 1)
        	{
        		return findK(nums1,nums2,(sz1+sz2)/2+1);
        	}
        	else
        	{
        		double t1 = findK(nums1,nums2,(sz1+sz2)/2);
                //cout<<t1<<endl;
        		double t2 = findK(nums1,nums2,(sz1+sz2)/2+1);
                //cout<<t2<<endl;
        		return (t1+t2)/2;
        	}

    }

    double findK(vector<int>& nums1, vector<int>& nums2,int k)
    {
    	int sz1 = nums1.size();
    	int sz2 = nums2.size();
    	int ptr1 = 0;
    	int ptr2 = 0;

    	while(k > 1)
    	{
    		int half = k / 2;
    		int val1,val2;
    		if(ptr1+half-1<sz1) val1 = nums1[ptr1+half-1];
    		else                val1 = INT_MAX;

    		if(ptr2+half-1<sz2) val2 = nums2[ptr2+half-1];
    		else                val2 = INT_MAX;

    		if(val1 <= val2)
    		{
    			ptr1 = ptr1 + half;
    		}
    		else
    		{
    			ptr2 = ptr2 + half;
    		}
    		k -= half;
    	}
        if(ptr1>=sz1) return nums2[ptr2];
        if(ptr2>=sz2) return nums1[ptr1];
    	return min(nums1[ptr1],nums2[ptr2]);

    }
};