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
        if(root->val > p->val && root->val > q->val)
        {
            return lowestCommonAncestor(root->left,p,q);
        }
        if(root->val < p->val && root->val < q->val)
        {
            return lowestCommonAncestor(root->right,p,q);
        }
        if(root == p)
        {
            if(q->val > root->val && search(root->right,q))
            {
                return p;
            }
            else if(q->val < root->val && search(root->left,q))
            {
                return p;
            }
        }
        if(root == q)
        {
            if(p->val > root->val && search(root->right,p))
            {
                return q;
            }
            else if(p->val < root->val && search(root->left,p))
            {
                return q;
            }
        }
        if(root->val > q->val && root->val < p->val)
        {
            if(search(root->left,q) && search(root->right,p)) return root;
        }
        if(root->val < q->val && root->val > p->val)
        {
            if(search(root->left,p) && search(root->right,q)) return root;
        }
        return NULL;        
    }
    
    bool search(TreeNode* root, TreeNode* tgt)
    {
        if(root==NULL) return false;
        if(root->val > tgt->val) return search(root->left,tgt);
        if(root->val < tgt->val) return search(root->right,tgt);
        return true;
    }
};