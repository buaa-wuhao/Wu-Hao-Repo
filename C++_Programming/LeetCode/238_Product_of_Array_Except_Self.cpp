lass Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        if(nums.empty()) return {};
		vector<int> res(nums.size());
		
		int prod = 1;
		int sz   = nums.size();
		for(int i=sz-1;i>=0;i--)
		{
			prod = prod*nums[i];
			res[i] = prod;
		}
		
		prod = 1;		
		for(int i=0;i<sz;i++)
		{
			if(i+1 < sz)
			{res[i] = prod*res[i+1];}
			else
			{res[i] = prod;}
			prod *= nums[i];   
		}
		return res;
    }
};
