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
    int kthSmallest(TreeNode* root, int k) {
		int kth;
		findk(root,k,kth);
		return kth;
	}
	
	bool findk(TreeNode* root,int& k,int& kth)
	{
		
		if(root==NULL) return false;
		if(findk(root->left,k,kth)) return true;
		if(k==1)
		{
			kth = root->val;
			return true;
		}
		k--;
		if(findk(root->right,k,kth)) return true;
		return false;
		
	}
};