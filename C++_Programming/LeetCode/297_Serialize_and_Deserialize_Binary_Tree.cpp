/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string str;
        if(root==NULL) return str;
        queue<TreeNode*> q;
        q.push(root);
        
        while(!q.empty())
        {
            TreeNode* node = q.front();
            q.pop();
            if(node)
            {
                str += (to_string(node->val) + ",");
            }
            else
            {
                str +=  "#,";
            }
            if(node==NULL) continue;
            q.push(node->left);
            q.push(node->right);
        }
        size_t pos = str.find_last_not_of("#,");
        cout << str.substr(0,pos+2) << endl;
        return str.substr(0,pos+2);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        if(data.empty()) return NULL;
        vector<TreeNode*> v_n;
        int start=0;
        
        for(int i=0;i < data.size();i++)
        {
            if(data[i]!=',') continue;
            string str = data.substr(start,i-start);
            start = i+1;
            if(str=="#") 
            {v_n.push_back(NULL);}
            else
            {
                TreeNode* node = new TreeNode(stoi(str));
                v_n.push_back(node);
            }
        }
        TreeNode* root = v_n[0];
		int end = 1;
        for(int i=0;i < v_n.size() && end < v_n.size();i++)
        {
			
			if(end < v_n.size() && v_n[i])
            {v_n[i]->left = v_n[end++];}
            
			if(end < v_n.size() && v_n[i])
            {v_n[i]->right = v_n[end++];} 
        }
        
        
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));