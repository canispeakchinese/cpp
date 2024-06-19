#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
  private:
    map<TreeNode *, int> max_record;
    int max_result;

    void cal(TreeNode *node) {
        if(!node) {
            return;
        }
        cal(node->left);
        cal(node->right);
        // 可能是当前节点加左右路径
        max_result = max(max_record[node->left] + max_record[node->right] + node->val, max_result);
        // 当前节点只能采用一个路径或者全不采用
        max_record[node] = max(node->val, max(max_record[node->left] + node->val, max_record[node->right] + node->val));
        // 这时候要再比一下大小
        max_result = max(max_record[node], max_result);
    }

  public:
    int maxPathSum(TreeNode *root) {
        max_result = -1000;
        max_record[nullptr] = 0;
        cal(root);
        return max_result;
    }
};