#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Solution {
private:
    bool intersect(vector<map<int, bool>>& records, int a, int b, int k) {
        int count = 0;
        for (const auto& pair : records[a]) {
            if(records[b].find(pair.first) != records[b].end()) {
                count++;
            }
        }
        return count >= k;
    }

public:
    int numberOfComponents(vector<vector<int>>&& properties, int k) {
        vector<map<int, bool>> records;
        for(int i=0; i<properties.size(); i++) {
            map<int, bool> record;
            for(auto v :properties[i]) {
                record[v] = true;
            }
            records.push_back(record);
        }

        vector<int> res;
        vector<vector<int>> edges;
        for(int i=0; i<properties.size(); i++) {
            res.push_back(0);
            vector<int> edge;
            for (int j=0; j<properties.size(); j++) {
                if(i == j) {
                    continue;
                }
                if(this->intersect(records, i, j, k)) {
                    edge.push_back(j);
                }
            }
            edges.push_back(edge);
        }

        int count = 0;

        for(int i=0; i<properties.size(); i++) {
            if(res[i]==0) {
                res[i]=++count;
                set<int> s;
                vector<int> ve;
                s.insert(i);
                ve.push_back(i);
                for(int j=0; j<ve.size(); j++) {
                    for(auto v:edges[ve[j]]) {
                        res[v] = res[i];
                        if(s.find(v) == s.end()) {
                            s.insert(v);
                            ve.push_back(v);
                        }
                    }
                }
            }
        }
        return count;
    }
};

int main() {
    Solution solute;
    cout << solute.numberOfComponents({{1, 2}, {1, 1}, {3, 4}, {4, 5}, {5, 6}, {7, 7}}, 1) << endl;
    return 0;
}