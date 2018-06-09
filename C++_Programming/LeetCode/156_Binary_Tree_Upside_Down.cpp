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
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        if(root==NULL) return NULL;
        TreeNode *new_root,*right;
        dfs(root,new_root,right);
        //cout<<right->val<<endl;
        return new_root;
    }
    
    void dfs(TreeNode* root, TreeNode*& new_root,TreeNode*& right)
    {
        if(root==NULL) 
        {
            new_root = NULL;
            right    = NULL;
            return;
        }              
        TreeNode *l_r,*new_l_root;
        dfs(root->left,new_l_root,l_r);
        
        TreeNode* r_r,*new_r_root;
        dfs(root->right,new_r_root,r_r);      
        
        if(l_r==NULL && new_l_root==NULL)
        {
            right = new_root = root;
            return;
        }        
        
        new_root = new_l_root;
        
        l_r->left = new_r_root;
        l_r->right = root;
        right = root; 
            
        root->left = NULL;
        root->right = NULL;        
     
        return ;        
        
    }
};