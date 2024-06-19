#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::vector;

class Solution {
  public:
    int findMaximizedCapital(int k, int w, vector<int> &&profits, vector<int> &&capital) {
        vector<pair<int, int>> ipoInfos;
        ipoInfos.reserve(ipoInfos.size());
        for(int i = 0; i < capital.size(); i++) {
            ipoInfos.push_back({capital[i], profits[i]});
        }
        std::sort(ipoInfos.begin(), ipoInfos.end());

        int ans = w;
        std::priority_queue<int> pq;
        for(int i = 0; i < ipoInfos.size(); i++) {
            if(ans >= ipoInfos[i].first) {
                pq.push(ipoInfos[i].second);
            } else if(!pq.empty()) {
                ans += pq.top();
                pq.pop();
                if(--k == 0) {
                    return ans;
                }
                i--;
            } else {
                return ans;
            }
        }
        while(k-- > 0 && !pq.empty()) {
            ans += pq.top();
            pq.pop();
        }
        return ans;
    }
};

int main(int argc, char *argv[]) {
    Solution s;
    std::cout << s.findMaximizedCapital(3, 10, vector<int>{3, 2, 1}, vector<int>{2, 1, 0}) << std::endl;
    return 0;
}