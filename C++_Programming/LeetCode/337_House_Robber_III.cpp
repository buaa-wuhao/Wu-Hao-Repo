class Solution {
public:
    int rob(TreeNode* root) {
        if(!root) return 0;
		int wth,wtho;
		rb(root,wth,wtho);
		return max(wth,wtho);
		
    }
	
	void rb(TreeNode* root, int& wth,int& wtho)
	{
		if(!root)
		{
			wth = 0;
			wtho = 0;
			return;
		}
		
		int l_wth,l_wtho;
		int r_wth,r_wtho;
		rb(root->left,l_wth,l_wtho);
		rb(root->right,r_wth,r_wtho);
		
		wth = root->val+l_wtho+r_wtho;
		wtho = max(l_wth+r_wth,l_wtho+r_wtho);
		wtho = max(wtho,l_wtho+r_wth);
		whto = max(wtho,r_wtho+l_wth);
		return;
	}
};