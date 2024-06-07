#include <iostream>
#include <map>
#include <set>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::set;
using std::vector;

class Solution {
  public:
    vector<int> findOrder(int numCourses, const vector<vector<int>> &prerequisites) {
        vector<int> res;
        res.reserve(numCourses);
        map<int, set<int>> prerequisites_map;
        map<int, set<int>> berequisites_map;
        for(auto &&v : prerequisites) {
            prerequisites_map[v[0]].insert(v[1]);
            berequisites_map[v[1]].insert(v[0]);
        }
        for(int i = 0; i < numCourses; ++i) {
            if(!prerequisites_map[i].size()) {
                res.push_back(i);
            }
        }
        for(int i = 0; i < res.size(); ++i) {
            for(auto &&v : berequisites_map[res[i]]) {
                prerequisites_map[v].erase(res[i]);
                if(!prerequisites_map[v].size()) {
                    res.push_back(v);
                }
            }
        }
        if(res.size() != numCourses) {
            return {};
        }
        return res;
    }
};

int main(int argc, char *argv[]) {
    Solution s;
    auto res = s.findOrder(2, {{1, 0}});
    for(auto &&i : res) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}