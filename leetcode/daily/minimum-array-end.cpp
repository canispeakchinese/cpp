#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
  private:
    vector<int> convertto2(int num) {
        vector<int> ans;
        while(num > 0) {
            ans.push_back(num % 2);
            num /= 2;
        }
        return ans;
    }

  public:
    long long minEnd(int n, int x) {
        // 可以认为x = 0000001001001
        // n-1 = 1001001
        // nums数组的第1个元素肯定等于x，之后的元素就是不停的往x中0的位置填空
        // 所以最后答案就是把n-1填进x的空格中去，也就是给______1__1__1填空
        // 最后答案就是(100)1(10)1(01)1转化为10进制的值
        auto x2 = convertto2(x);
        auto n2 = convertto2(n - 1);
        for(int i, j = 0;; i++) {
            if(j == n2.size()) {
                break;
            } else if(i == x2.size()) {
                x2.push_back(n2[j++]);
            } else if(x2[i] == 0) {
                x2[i] = n2[j++];
            }
        }
        long long ans = 0;
        for(int i = 0; i < x2.size(); i++) {
            if(x2[i]) {
                ans += (long long)1 << i;
            }
        }
        return ans;
    }
};