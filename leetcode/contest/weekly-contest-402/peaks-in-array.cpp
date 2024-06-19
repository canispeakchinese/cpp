#include <iostream>
#include <vector>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(int n) : n(n) { tree.resize(4 * n); }

    // 建树
    void build(const vector<int> &arr) {
        this->arr = arr;
        build(0, 0, n - 1);
    }

    // 单点更新pos 改成 val
    void update(int pos, int val) {
        arr[pos] = val;
        update(0, 0, arr.size() - 1, pos);
    }

    // 区间查询 [l, r] 的和
    int query(int l, int r) { return query(0, 0, n - 1, l, r); }

  private:
    int n;
    vector<int> tree, arr;

    int get_extra(int mid, int start, int end) {
        int extra = 0;
        if(mid > start && mid < end) {
            if(arr[mid] > arr[mid - 1] && arr[mid] > arr[mid + 1]) {
                extra++;
            }
        }
        if(mid + 1 < end && mid + 1 > start) {
            if(arr[mid + 1] > arr[mid] && arr[mid + 1] > arr[mid + 2]) {
                extra++;
            }
        }
        return extra;
    }

    void build(int node, int start, int end) {
        if(start == end) {
            tree[node] = 0;
        } else {
            int mid = (start + end) / 2;
            build(2 * node + 1, start, mid);
            build(2 * node + 2, mid + 1, end);

            tree[node] = tree[2 * node + 1] + tree[2 * node + 2] + get_extra(mid, start, end);
        }
    }

    void update(int node, int start, int end, int pos) {
        if(start > end || start > pos || end < pos || start == pos) {
            return;
        }
        int mid = (start + end) / 2;
        if(pos <= mid) {
            update(2 * node + 1, start, mid, pos);
        } else {
            update(2 * node + 2, mid + 1, end, pos);
        }
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2] + get_extra(mid, start, end);
    }

    int query(int node, int start, int end, int l, int r) {
        if(start > end || start > r || end < l) {
            return 0;
        }
        if(start == l && end == r) {
            return tree[node];
        }
        int mid = (start + end) / 2, leftSum = 0, rightSum = 0;
        if(mid >= l) {
            leftSum = query(2 * node + 1, start, mid, l, r > mid ? mid : r);
        }
        if(mid + 1 <= r) {
            rightSum = query(2 * node + 2, mid + 1, end, l < mid + 1 ? mid + 1 : l, r);
        }
        return leftSum + rightSum + get_extra(mid, l, r);
    }
};

class Solution {
  public:
    vector<int> countOfPeaks(vector<int> &&nums, vector<vector<int>> &&queries) {
        SegmentTree tree(nums.size());
        tree.build(nums);

        vector<int> res;
        res.reserve(queries.size());
        for(auto &&i : queries) {
            if(i[0] == 1) {
                res.push_back(tree.query(i[1], i[2]));
            } else if(i[0] == 2) {
                tree.update(i[1], i[2]);
            }
        }
        return res;
    }
};

int main() {
    auto ans = Solution().countOfPeaks({8, 10, 4, 3, 5}, {{2, 4, 1}, {1, 0, 1}, {2, 0, 5}});
    for(auto &&i : ans) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}