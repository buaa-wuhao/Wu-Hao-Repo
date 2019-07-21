class Solution {
public:
    bool isPalindrome(ListNode* head) {
		if(head==NULL) return true;
		ListNode* start = head;
		
		return dfs(start,head);
		
	}
	
	bool dfs(ListNode*& start,ListNode* ptr)
	{
		if(ptr->next==NULL)
		{
			if(start->val==ptr->val)
			{
				start = start->next;
				return true;
			}
			return false;
		}
		
		if(dfs(start,ptr->next))
		{
			if(start->val==ptr->val)
			{
				start = start->next;
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
};

