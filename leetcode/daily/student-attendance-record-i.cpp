#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Solution {
  public:
    bool checkRecord(string s) {
        int A = 0, L = 0;
        for(auto &&v : s) {
            if(v == 'A') {
                if(++A > 1) {
                    return false;
                }
            }

            if(v == 'L') {
                if(++L > 2) {
                    return false;
                }
            } else {
                L = 0;
            }
        }
        return true;
    }
};