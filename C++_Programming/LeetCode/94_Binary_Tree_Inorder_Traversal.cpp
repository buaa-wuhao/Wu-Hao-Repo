/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
 vector<int> inorderTraversal(TreeNode* root) {
	if(root == NULL) return {};
	vector<int> res;
	stack<TreeNode*> stk;
	stk.push(root);
	size_t prv = 0;
	while(!stk.empty())
	{
		TreeNode* tp = stk.top();
		if(tp->left==NULL || stk.size()<prv)
		{
			res.push_back(tp->val);
			prv = stk.size();            
			stk.pop();
			if(tp->right) stk.push(tp->right);
		}
		else
		{
			stk.push(tp->left);
		}
	}
	
	return res;
 }
};
 
 
 /**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        if(root==NULL) return {};
        vector<int> ans;
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        
        while(cur || !stk.empty())
        {
            while(cur)
            {
                stk.push(cur);
                cur = cur->left;
            }
            
            cur = stk.top();
            ans.push_back(cur->val);
            stk.pop();
            cur = cur->right;
        }
        return ans;
    }
};

