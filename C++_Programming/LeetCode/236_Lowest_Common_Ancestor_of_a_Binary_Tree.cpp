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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(root==NULL) return NULL;
        bool p_fnd=false,q_fnd=false;
        TreeNode* llc = lca(root,p,q,p_fnd,q_fnd);
        return llc;
    }
    
    TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q,
                  bool& p_fnd, bool& q_fnd)
    {
        if(root==NULL) return NULL;
        bool l_p_fnd = false,l_q_fnd = false;
        TreeNode* lft = lca(root->left,p,q,l_p_fnd,l_q_fnd);
        if(lft) return lft;
        bool r_p_fnd = false,r_q_fnd = false;
        TreeNode* rit = lca(root->right,p,q,r_p_fnd,r_q_fnd);
        if(rit) return rit;
        
        if(l_p_fnd && r_q_fnd) return root;
        if(r_p_fnd && l_q_fnd) return root;
        if(root==p && (l_q_fnd || r_q_fnd)) return root;
        if(root==q && (l_p_fnd || r_p_fnd)) return root;
        
        p_fnd = (l_p_fnd || r_p_fnd || root==p);
        q_fnd = (l_q_fnd || r_q_fnd || root==q);
        return NULL;
        
    }
};