#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

typedef long long ll;

class IntensitySegments {
  public:
    IntensitySegments(){};

    // 设置从from到to的强度增加amount
    // 时间复杂度为o(lgn)
    void add(ll from, ll to, ll amount) {
        if(from > to) {
            cout << "unexcept param, ignore" << endl;
            return;
        }
        // 如果加的强度为0，那就是啥也不操作
        if(amount == 0) {
            return;
        }

        segment_infos[from] += amount;
        segment_infos[to] -= amount;

        // 如果加的值为0，则无意义，可以直接删除，这样可以合并区间
        erase_zero(from, to);
    }

    // 设置从from到to的强度置为amount
    // 极端情况下时间复杂度为o(nlgn)
    void set(ll from, ll to, ll amount) {
        if(from > to) {
            cout << "unexcept param, ignore" << endl;
            return;
        }
        ll sum_before = 0, sum_between = 0;

        vector<ll> erase_values;
        // 前面一直累加到from之前（包含from）的值为sum_before，要在from处加上amount-sum_before,才能保证from初为amount
        // from（不含）和to（不含）中间的值为sum_between，中间的这些值都需要被移除掉，才能保证从from到to都是amount
        for(const auto &segment_info : segment_infos) {
            if(segment_info.first <= from) {
                sum_before += segment_info.second;
            } else if(segment_info.first < to) {
                sum_between += segment_info.second;
                erase_values.push_back(segment_info.first);
            } else {
                break;
            }
        }
        // 那么在from处就要再加amount-sum_before_from，加完之后正好为amount
        segment_infos[from] += amount - sum_before;
        // from和to中间的值全都删掉，保证后续一直是amount
        for(const auto &element : erase_values) {
            segment_infos.erase(element);
        }
        // 在to处需要加上前面移除掉的sum_between，再减掉前面加上的amount - sum_before，才能保证后面的值不会受到影响
        segment_infos[to] += sum_between - (amount - sum_before);

        // 如果加的值为0，则无意义，可以直接删除，这样可以合并区间
        erase_zero(from, to);
    }

    // 时间复杂度o(nlgn)
    void toString() {
        ll sum = 0;

        ostringstream oss;
        oss << "[";
        for(const auto &segment : segment_infos) {
            sum += segment.second;
            oss << "[" << segment.first << "," << sum << "]";
        }
        oss << "]";
        cout << oss.str() << endl;
    }

    bool verify(map<ll, ll> except) {
        if(except.size() != segment_infos.size()) {
            return false;
        }

        for(const auto &segment : segment_infos) {
            if(except.find(segment.first) == except.end() || except.find(segment.first)->second != segment.second) {
                return false;
            }
        }

        return true;
    };

  private:
    inline void erase_zero(ll from, ll to) {
        if(segment_infos[from] == 0) {
            segment_infos.erase(from);
        }
        if(segment_infos[to] == 0) {
            segment_infos.erase(to);
        }
    }

    /*
        前缀和算法解决问题：
            map<k, v>表示从k点开始往后都加v
    */
    map<ll, ll> segment_infos;
};

int main() {
    IntensitySegments *segments = new IntensitySegments();

    // 测试合并
    segments->add(10, 30, 1);
    segments->add(80, 100, 1);
    segments->add(30, 80, 1);
    segments->toString();
    if(!segments->verify(map<ll, ll>{{10, 1}, {100, -1}})) {
        cout << "测试合并失败" << endl;
    } else {
        cout << "测试合并成功" << endl;
    }

    segments->add(10, 100, -1);
    segments->toString();
    if(!segments->verify(map<ll, ll>{})) {
        cout << "测试清空失败" << endl;
    } else {
        cout << "测试清空成功" << endl;
    }

    segments->add(100, 10, -1);
    segments->toString();
    if(!segments->verify(map<ll, ll>{})) {
        cout << "测试异常add输入失败" << endl;
    } else {
        cout << "测试异常add输入成功" << endl;
    }

    segments->set(100, 10, -1);
    segments->toString();
    if(!segments->verify(map<ll, ll>{})) {
        cout << "测试异常set输入失败" << endl;
    } else {
        cout << "测试异常set输入成功" << endl;
    }

    segments->add(10, 100, 12345);
    segments->add(55, 61, 3);
    segments->set(50, 60, -12345);
    segments->toString();
    if(!segments->verify(map<ll, ll>{{10, 12345}, {50, -12345 * 2}, {60, 12345 * 2 + 3}, {61, -3}, {100, -12345}})) {
        cout << "测试set失败" << endl;
    } else {
        cout << "测试set成功" << endl;
    }

    delete segments;
    return 0;
}