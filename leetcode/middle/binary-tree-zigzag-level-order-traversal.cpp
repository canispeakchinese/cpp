#include <iostream>
#include <vector>

using std::vector;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
  public:
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        if(!root) {
            return {};
        }

        vector<vector<int>> ans;
        vector<vector<TreeNode *>> record;
        ans.reserve(2000);
        record.reserve(2000);
        record.push_back({root});
        ans.push_back({root->val});

        for(int i = 0; i < record.size(); i++) {
            vector<int> temp1;
            vector<TreeNode *> temp2;
            temp1.reserve(record[i].size() * 2);
            temp2.reserve(record[i].size() * 2);
            for(int j = record[i].size() - 1; j >= 0; j--) {
                if(i % 2) {
                    if(record[i][j]->left) {
                        temp1.push_back(record[i][j]->left->val);
                        temp2.push_back(record[i][j]->left);
                    }
                    if(record[i][j]->right) {
                        temp1.push_back(record[i][j]->right->val);
                        temp2.push_back(record[i][j]->right);
                    }
                } else {
                    if(record[i][j]->right) {
                        temp1.push_back(record[i][j]->right->val);
                        temp2.push_back(record[i][j]->right);
                    }
                    if(record[i][j]->left) {
                        temp1.push_back(record[i][j]->left->val);
                        temp2.push_back(record[i][j]->left);
                    }
                }
            }
            if(temp1.size()) {
                ans.push_back(temp1);
                record.push_back(temp2);
            }
        }
        return ans;
    }
};