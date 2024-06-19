#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    int firstMissingPositive(vector<int> &nums) {
        vector<int> record(nums.size() + 2, 0);
        for(auto &&v : nums) {
            if(v > 0 && v <= nums.size()) {
                record[v] = 1;
            }
        }
        for(int i = 1; i < nums.size() + 1; i++) {
            if(!record[i]) {
                return i;
            }
        }
        return -1;
    }
};