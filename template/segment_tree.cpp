#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
  public:
    SegmentTree(int n) : n(n) {
        tree.resize(4 * n);
        lazy.resize(4 * n, 0);
    }

    // 建树
    void build(const vector<int> &arr) { build(0, 0, n - 1, arr); }

    // 区间更新 [l, r] 加上 val
    void update(int l, int r, int val) { update(0, 0, n - 1, l, r, val); }

    // 区间查询 [l, r] 的和
    int query(int l, int r) { return query(0, 0, n - 1, l, r); }

  private:
    int n;
    vector<int> tree, lazy;

    void build(int node, int start, int end, const vector<int> &arr) {
        if(start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(2 * node + 1, start, mid, arr);
            build(2 * node + 2, mid + 1, end, arr);
            tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
        }
    }

    void propagate(int node, int start, int end) {
        if(lazy[node] != 0) {
            tree[node] += (end - start + 1) * lazy[node];
            if(start != end) {
                lazy[2 * node + 1] += lazy[node];
                lazy[2 * node + 2] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update(int node, int start, int end, int l, int r, int val) {
        propagate(node, start, end);
        if(start == l && end == r) {
            tree[node] += (end - start + 1) * val;
            if(start != end) {
                lazy[2 * node + 1] += val;
                lazy[2 * node + 2] += val;
            }
            return;
        }
        int mid = (start + end) / 2;
        if(l <= mid) {
            update(2 * node + 1, start, mid, l, mid < r ? mid : r, val);
        }
        if(r >= mid + 1) {
            update(2 * node + 2, mid + 1, end, mid + 1 > l ? mid + 1 : l, r, val);
        }
        tree[node] = tree[2 * node + 1] + tree[2 * node + 2];
    }

    int query(int node, int start, int end, int l, int r) {
        propagate(node, start, end);
        if(start == l && end == r) {
            return tree[node];
        }
        int mid = (start + end) / 2, leftSum = 0, rightSum = 0;
        if(l <= mid) {
            leftSum = query(2 * node + 1, start, mid, l, mid < r ? mid : r);
        }
        if(r >= mid + 1) {
            rightSum = query(2 * node + 2, mid + 1, end, mid + 1 > l ? mid + 1 : l, r);
        }
        return leftSum + rightSum;
    }
};

int main() {
    vector<int> arr = {1, 3, 5, 7, 9, 11};
    SegmentTree segTree(arr.size());
    segTree.build(arr);

    cout << "Initial sum of values in range [1, 3]: " << segTree.query(1, 3) << endl;

    segTree.update(1, 5, 10);
    cout << "Sum of values in range [1, 3] after update: " << segTree.query(1, 3) << endl;

    segTree.update(3, 5, 5);
    cout << "Sum of values in range [3, 5] after another update: " << segTree.query(3, 5) << endl;

    return 0;
}
