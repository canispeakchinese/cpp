#include <iostream>

using std::cin;
using std::cout;

class Solution {
  public:
    int rangeBitwiseAnd(int left, int right) {
        int ans = 0;

        long long st = 1;
        while(st <= right) {
            // 必须要保证从left到right不会经历一个完成的st周期，而且left和right在st位上都要为1，才能说明从left到right都在st位上为1
            if(right - left < st && (right & st) != 0 && (left & st) != 0) {
                ans |= st;
            }
            st <<= 1;
        }

        return ans;
    }
};

int main(int argc, char *argv[]) {
    Solution s;
    cout << s.rangeBitwiseAnd(2, 2) << std::endl;
    return 0;
}