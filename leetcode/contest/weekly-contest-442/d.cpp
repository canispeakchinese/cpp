#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Solution {
private:
    vector<pair<int,int>> i24;
    long long minOperation(int l, int r) {
        long long ans = 0;
        for(auto p: i24) {
            // cout << l << " " << p.first << endl;
            if(l<=p.first) {
                // 如果第一次小于等于
                if(ans == 0) {
                    // 如果右边也小于等于，则直接返回他两中间数的数量*操作次数
                    if(r<=p.first) {
                        return (long long)(r-l+1)*(long long)p.second;
                    }
                    // 计算上从l到这次的数的数量*操作次数
                    ans += (long long)(p.first-l+1)*(long long)p.second;
                } else {
                    // 计算上从上个循环到这个循环的数字数量*操作次数
                    ans += (long long)(p.first+1-(p.first+1)/4)*(long long)p.second;
                    // cout << ans << endl;
                    // 如果右边小于等于了，则减掉差值*操作次数
                    if (r<=p.first) {
                        ans = ans - (long long)(p.first-r)*(long long)p.second;
                        // cout << ans << endl;
                        return ans;
                    }
                }
            }
        }
        return 0;
    }
public:
    Solution() {
        int opeNum = 1;
        long long st = 4;
        for(; st<1e9; st*=4) {
            i24.push_back(pair<long long,int>{int(st-1), opeNum++}); // 小于等于该数的，只需要操作opeNum次
        }
        i24.push_back(pair<long long,int>{st-1, opeNum}); // 小于等于该数的，只需要操作1次
    }
    long long minOperations(vector<vector<int>>&& queries) {
        long long ans = 0;
        for(auto query: queries) {
            long long oneAns = minOperation(query[0], query[1]);
            // cout << oneAns << endl;
            ans += oneAns/2 + oneAns%2;
        }
        return ans;
    }
};

int main() {
    Solution solute;
    cout << solute.minOperations({{1, 2}, {2, 4}}) << endl;
}