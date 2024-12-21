#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    long long maximumTotalDamage(vector<int> &power) {
        map<int, long long> m;
        map<int, bool> used;
        for(auto &&i : power) {
            m[i] += i;
        }

        vector<pair<int, long long>> v;
        v.reserve(m.size());
        for(auto &&i : m) {
            v.push_back({i.first, i.second});
        }
        sort(v.begin(), v.end());

        vector<long long> dp(v.size());
        vector<long long> max(v.size());
        dp[0] = max[0] = v[0].second;
        for(int i = 1; i < v.size(); i++) {
            dp[i] = v[i].second;
            for(int j = i - 1; j >= 0; j--) {
                if(v[j].first < v[i].first - 2) {
                    dp[i] += max[j];
                    break;
                }
            }
            max[i] = dp[i] > max[i - 1] ? dp[i] : max[i - 1];
        }

        long long ans = dp[0];
        for(int i = 1; i < v.size(); i++) {
            if(dp[i] > ans) {
                ans = dp[i];
            }
        }
        return ans;
    }
};

int main(int argc, char const *argv[]) {
    Solution s;
    vector<int> v = {5, 9, 2, 10, 2, 7, 10, 9, 3, 8};
    cout << s.maximumTotalDamage(v) << endl;
    return 0;
}