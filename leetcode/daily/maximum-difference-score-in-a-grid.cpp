#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// 这块其实最终值就是终点-起点，因为其实可以理解是一路置换过来的
// 然后怎么找出最大的终点-起点呢？
// 我的思路是从上往下去枚举每个点左上方的最小点
// 最终答案就是最大的当前点-左上方最小点的值
class Solution {
  public:
    int maxScore(vector<vector<int>> &grid) {
        int ans = -1000000;
        vector<vector<int>> min_record(grid.size());
        for(int i = 0; i < grid.size(); i++) {
            min_record[i] = vector<int>(grid[i].size());
            for(int j = 0; j < grid[i].size(); j++) {
                // 绝对不能用
                if(i == 0 && j == 0) {
                    min_record[i][j] = 1000000;
                } else if(i == 0) {
                    min_record[i][j] = min(min_record[i][j - 1], grid[i][j - 1]);
                } else if(j == 0) {
                    min_record[i][j] = min(min_record[i - 1][j], grid[i - 1][j]);
                } else {
                    min_record[i][j] =
                        min(min_record[i][j - 1], min(min_record[i - 1][j], min(grid[i][j - 1], grid[i - 1][j])));
                }
                ans = max(ans, grid[i][j] - min_record[i][j]);
            }
        }
        return ans;
    }
};