#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    int maxmiumScore(vector<int> &cards, int cnt) {
        int ans = 0;

        int last_odd = 0, last_even = 0, ans_odd = 0, ans_even = 0;
        sort(cards.begin(), cards.end(), greater<int>());
        for(int i = 0; i < cnt; i++) {
            ans += cards[i];
            if(cards[i] % 2) {
                last_odd = cards[i];
            } else {
                last_even = cards[i];
            }
        }

        if(ans % 2) {
            if(last_odd) {
                for(int i = cnt; i < cards.size(); i++) {
                    if(cards[i] % 2 == 0) {
                        ans_odd = ans - last_odd + cards[i];
                        break;
                    }
                }
            }
            if(last_even) {
                for(int i = cnt; i < cards.size(); i++) {
                    if(cards[i] % 2) {
                        ans_odd = ans - last_even + cards[i];
                        break;
                    }
                }
            }
            return max(ans_odd, ans_even);
        } else {
            return ans;
        }

        return 0;
    }
};