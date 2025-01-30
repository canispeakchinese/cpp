#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
public:
    bool isPossibleToSplit(vector<int>&& nums) {
        map<int, int> record;
        for (auto &&i : nums){
            if(++record[i]> 2) {
                return false;
            }
        }
        return true;
    }
};

int main(int argc, char** argv) {
    cout << Solution().isPossibleToSplit({1, 1, 1, 1}) << endl;
    vector<int> case2{1, 1, 2, 2};
    cout << Solution().isPossibleToSplit(move(case2)) << endl;
    cout << case2.size() << endl;
    return 0;
};