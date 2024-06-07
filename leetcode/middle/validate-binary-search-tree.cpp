#include <iostream>

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
    bool check(TreeNode *node, int64_t min, int64_t max) {
        if(node == nullptr)
            return true;
        if(node->val <= min || node->val >= max)
            return false;
        return check(node->left, min, node->val) && check(node->right, node->val, max);
    }
    bool isValidBST(TreeNode *root) { return check(root, LONG_MIN, LONG_MAX); }
};