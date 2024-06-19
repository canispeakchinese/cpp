#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    long long countCompleteDayPairs(vector<int> &hours) {
        map<int, int> m;
        for(auto &&i : hours) {
            m[i % 24]++;
        }

        long long ans = 0;
        for(int i = 0; i <= 12; i++) {
            if(i == 24 - i || i == 0) {
                ans += (long long)m[i] * (long long)(m[i] - 1) / (long long)(2);
            } else {
                ans += (long long)m[i] * (long long)(m[24 - i]);
            }
        }
        return ans;
    }
};