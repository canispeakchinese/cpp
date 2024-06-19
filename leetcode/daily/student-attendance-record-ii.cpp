#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    int checkRecord(int n) {
        // dp[i][j][k] = 前i天总共j次缺勤连上第i天连续k天迟到
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(2, vector<int>(3, 0)));
        dp[1][0][0] = 1;
        dp[1][1][0] = 1;
        dp[1][0][1] = 1;
        for(int i = 2; i <= n; i++) {
            // 首先考虑当天迟到的情况，这时候意味着只会变动最后一位：
            dp[i][0][2] = dp[i - 1][0][1];
            dp[i][1][2] = dp[i - 1][1][1];
            dp[i][0][1] = dp[i - 1][0][0];
            dp[i][1][1] = dp[i - 1][1][0];

            // 排除掉当天迟到的情况，这时候连续迟到天数就一定为0了

            // 其次考虑当天缺勤，总共只能缺勤一次，所以之前肯定没有缺勤过
            dp[i][1][0] = ((dp[i - 1][0][0] + dp[i - 1][0][1]) % int(1e9 + 7) + dp[i - 1][0][2]) % int(1e9 + 7);
            // 再次考虑当天正常，这时候总缺勤还是可以为1
            dp[i][1][0] =
                (((dp[i - 1][1][0] + dp[i - 1][1][1]) % int(1e9 + 7) + dp[i - 1][1][2]) % int(1e9 + 7) + dp[i][1][0]) %
                int(1e9 + 7);
            // 也可以为0
            dp[i][0][0] = ((dp[i - 1][0][0] + dp[i - 1][0][1]) % int(1e9 + 7) + dp[i - 1][0][2]) % int(1e9 + 7);
        }
        return (((((dp[n][0][0] + dp[n][0][1]) % int(1e9 + 7) + dp[n][0][2]) % int(1e9 + 7) + dp[n][1][0]) %
                     int(1e9 + 7) +
                 dp[n][1][1]) %
                    int(1e9 + 7) +
                dp[n][1][2]) %
               int(1e9 + 7);
    }
};