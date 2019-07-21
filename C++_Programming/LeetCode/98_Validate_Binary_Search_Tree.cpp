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
    bool isValidBST(TreeNode* root) {
        long _max,_min;
        return isBST(root,_max,_min);
    }
    
    bool isBST(TreeNode* root,long& _max,long& _min)
    {
        if(root==NULL)
        {
            _min = LONG_MAX;
            _max = LONG_MIN;
            return true;
        }
        long l_max,l_min;
        bool left_true = isBST(root->left,l_max,l_min);
        if(left_true==false || l_max>=root->val) return false;
        
        long r_max,r_min;
        bool right_true = isBST(root->right,r_max,r_min);
        if(right_true==false || r_min<= root->val) return false;
        
        _min = min(l_min,(long)root->val);
        _max = max(r_max,(long)root->val);
        return true;
        
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
    bool isValidBST(TreeNode* root) {
        long pre = LONG_MIN;
        return isbst(root,pre);
    }
    
    bool isbst(TreeNode* root, long& pre)
    {
        if(root==NULL) return true;
        if(isbst(root->left,pre)==false) return false;
        if(pre >= root->val) return false;
        pre = root->val;
        if(isbst(root->right,pre)==false) return false;
        return true;
    }
};