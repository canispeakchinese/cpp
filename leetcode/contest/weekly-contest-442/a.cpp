#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Solution {
public:
    int maxContainers(int n, int w, int maxWeight) {
        return min(n*n, maxWeight/w);
    }
};