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
    int height(TreeNode* root)
    {
        if(root==NULL) return 0;
        return height(root->left)+1;
    }
    
    int countNodes(TreeNode* root) {
        if(root==NULL) return 0;
        int h = height(root);
        //cout << root->val << " " << h << endl;
        if(height(root->right)==h-1)
        {
            return ((1<<(h-1)) + countNodes(root->right));    
        }
        else
        {
            return ((1<<(h-2)) + countNodes(root->left));
        }
    }
};