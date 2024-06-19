#include <iostream>
#include <vector>

using namespace std;

class Solution {
  public:
    vector<vector<int>> modifiedMatrix(vector<vector<int>> &matrix) {
        vector<int> column_max;
        column_max.reserve(matrix.size());

        for(int i = 0; i < matrix.size(); i++) {
            for(int j = 0; j < matrix[i].size(); j++) {
                if(i == 0) {
                    column_max.push_back(matrix[i][j]);
                } else {
                    column_max[j] = max(column_max[j], matrix[i][j]);
                }
            }
        }

        for(int i = 0; i < matrix.size(); i++) {
            for(int j = 0; j < matrix[i].size(); j++) {
                matrix[i][j] = matrix[i][j] == -1 ? column_max[j] : matrix[i][j];
            }
        }

        return matrix;
    }
};

int main(int argc, char *argv[]) {
    Solution solution;
    vector<vector<int>> matrix = {{1, 2, -1}, {4, -1, 6}, {7, 8, 9}};
    vector<vector<int>> modified = solution.modifiedMatrix(matrix);

    for(auto row : modified) {
        for(int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }

    return 0;
}