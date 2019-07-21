
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        if(nums.empty()) return 1;
		int n = nums.size();
		for(int i=0;i<nums.size();i++)
		{
			if(nums[i]>n || nums[i]<1) continue;
			if(nums[i]==i+1) continue;
			//没有想到这种情况,例如[1,1] ,没有下面这一行，就死循环了
            if(nums[nums[i]-1]==nums[i]) continue;
			
			int tmp = nums[nums[i]-1];
			nums[nums[i]-1] = nums[i];
			nums[i] = tmp;
			i--;
		}
		
		for(int i=0;i<nums.size();i++)
		{
			//第一次做的时候，return nums[i]，不对！
            if(nums[i]!=i+1) return i+1;
		}
		return n+1;
    }
};