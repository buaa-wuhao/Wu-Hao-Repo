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
    bool isBalanced(TreeNode* root) {
        int dth = 0;
        return isB(root,dth);
    }
    
    bool isB(TreeNode* root, int& dth)
    {
        if(root==NULL)
        {
            dth = 0;
            return true;
        }
        int l_dth;
        if(isB(root->left,l_dth)==false) return false;
        
        int r_dth;
        if(isB(root->right,r_dth)==false) return false;
        if(abs(r_dth-l_dth)>1) return false;
        dth = max(l_dth,r_dth)+1;
        return true;
    }
};