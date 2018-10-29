#include <iostream>
#include <queue>
#include <vector>
using namespace std;
struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x):val(x),left(NULL),right(NULL) {}
};
/*
class mycomp
{
    bool operator()(const pair<double,int>& lhs,const pair<double,int>& rhs)
    {
        return lhs.first < rhs.first;
    }
};*/

class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        if(root==NULL || k<=0) return {};
        priority_queue<pair<double,int>,vector<pair<double,int>>,less<pair<double,int>>> pq;

        //priority_queue<double> pq;
        traverse(root,k,pq,target);

        vector<int> res;
        while(!pq.empty())
        {
            res.push_back(pq.top().second);
            pq.pop();
        }

        return res;

    }

    void traverse(TreeNode* root, int k,
                  priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>> >& pq,
                  double tgt)
    {

        if(root==NULL) return;
        traverse(root->left,k,pq,tgt);
        double diff = abs(tgt-root->val);

        if(pq.size()<k)
        {
            pq.push({diff,root->val});
        }
        else
        {
            if(pq.top().first > diff)
            {
                pq.pop();
                pq.push({diff,root->val});
            }
        }

        traverse(root->right,k,pq,tgt);
    }
};
int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
