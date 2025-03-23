#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class Solution {
public:
    long long minTime(vector<int>&& skill, vector<int>&& mana) {
        vector<vector<long long>> records(mana.size()); // 第i个药被第j个巫师完成的时间点，不考虑之前药的完成时间点
        for(int i=0; i< mana.size(); i++) {
            vector<long long> record(skill.size());
            cout << "第" << i << "瓶要各个巫师的制造时间：";
            for(int j=0; j<skill.size();j++) {
                record[j] = (long long)skill[j]*(long long)mana[i];
                cout << " " << record[j];
                if(j > 0) {
                    record[j] += record[j-1];
                }
            }
            cout << endl;
            records[i] = record;
        }
        for(int i=0; i<records.size(); i++) {
            cout << "第" << i << "个药的具体制作时间：";
            for(int j=0; j<records[i].size(); j++) {
                cout << " " << records[i][j];
            }
            cout << endl;
        }
        for(int i=1; i<records.size(); i++) {
            long long before = 0; // 前置步骤必须要花费的时间
            for(int j=0; j<records[i].size(); j++) {
                long long start_time = max(before, records[i-1][j]); // 开始时间等于前置步骤必须要花费的时间和做完上一个药的时间的最小值
                // cout << "start time: " << start_time << endl; 
                before = start_time + (long long)mana[i]*(long long)skill[j]; // 计算下一个步骤的前置步骤必须要花费的时间
                // cout << "new before: " << before << endl; 
            }
            // 最后的before时间就是当前这个药完成的时间点
            // cout << "before: " << before << endl;
            long long add = before - records[i][records[i].size()-1]; // 完成第一个药导致的下一个药需要增加的时间
            cout << "add: " << add << endl;
            for(int j=0; j<records[i].size(); j++) {
                records[i][j] += add;
            }
        }
        return records[records.size()-1][records[records.size()-1].size()-1];
    }
};

int main() {
    Solution solute;
    cout << solute.minTime({1, 5, 2, 4}, {5, 1, 4, 2}) << endl;
    return 0;
}