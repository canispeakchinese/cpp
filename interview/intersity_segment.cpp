#include <iostream>
#include <limits.h>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

typedef long long ll;

class Node {
  public:
    Node() = delete;
    Node(ll position, ll value, Node *prev, Node *next) : position(position), value(value), prev(prev), next(next) {}

    Node *prev, *next;
    ll position;  // position记录当前点
    ll value;     // value记录当前点的值
};

class IntensitySegments {
  public:
    IntensitySegments(){};
    ~IntensitySegments() {
        Node *node = root;
        while(node != NULL && node->next != NULL) {
            node = node->next;
            erase_node(node->prev->position, node->prev);
        }

        if(node != NULL) {
            erase_node(node->position, node);
        }
    }

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

        // 当前线段信息为空，初始化两个节点，保证后面不需要考虑线段信息为空的问题
        if(segment_infos.empty()) {
            init(from, to, amount);
            return;
        }

        Node *from_node = add_value(from, amount);
        Node *to_node = add_value(to, -amount);

        // 如果节点处终值为0，则无意义，可以直接删除，这样可以合并区间
        erase_zero(from, from_node);
        if(to != from) {
            erase_zero(to, to_node);
        }
    }

    // 设置从from到to的强度置为amount
    // 极端情况下时间复杂度为o(nlgn)
    void set(ll from, ll to, ll amount) {
        if(from > to) {
            cout << "unexcept param, ignore" << endl;
            return;
        }

        // 如果当前线段信息为空，那么set等价于add，因此同理后续分支无需考虑线段信息为空的情况
        if(segment_infos.empty()) {
            add(from, to, amount);
            return;
        }

        ll sum_before = 0, sum_between = 0;

        vector<Node *> erase_values;
        // 前面一直累加到from之前（包含from）的值为sum_before，要在from处加上amount-sum_before,才能保证from初为amount
        // from（不含）和to（不含）中间的值为sum_between，中间的这些值都需要被移除掉，才能保证从from到to都是amount
        // Note: 该段逻辑优化到o(n)
        Node *node = root;
        while(node != NULL) {
            if(node->position <= from) {
                sum_before += node->value;
            } else if(node->position < to) {
                sum_between += node->value;
                erase_values.push_back(node);
            } else {
                break;
            }
            node = node->next;
        }
        // 那么在from处就要再加amount-sum_before_from，加完之后正好为amount
        Node *from_node = add_value(from, amount - sum_before);
        // from和to中间的值全都删掉，保证后续一直是amount
        for(const auto &element : erase_values) {
            erase_node(element->position, element);
        }
        // 在to处需要加上前面移除掉的sum_between，再减掉前面加上的amount - sum_before，才能保证后面的值不会受到影响
        Node *to_node = add_value(to, sum_between - (amount - sum_before));

        // 如果加的值为0，则无意义，可以直接删除，这样可以合并区间
        erase_zero(from, from_node);
        if(from != to) {
            erase_zero(to, to_node);
        }
    }

    // Note:函数时间复杂度优化到o(n)
    void toString() {
        ll sum = 0;
        Node *node = root;

        ostringstream oss;
        oss << "[";
        while(node != NULL) {
            sum += node->value;
            oss << "[" << node->position << "," << sum << "]";
            node = node->next;
        }
        oss << "]";
        cout << oss.str() << endl;
    }

    bool verify(map<ll, ll> except) {
        if(except.size() != segment_infos.size()) {
            return false;
        }

        for(const auto &segment : segment_infos) {
            if(except.find(segment.first) == except.end() ||
               except.find(segment.first)->second != segment.second->value) {
                return false;
            }
        }

        return true;
    };

  private:
    // 时间复杂度o(lgn)
    Node *add_value(ll position, ll amount) {
        // 如果position比当前最大的节点还大，那么插入一个最大的节点
        // 如果position节点正好在map中，那么更新值就可以了
        // 如果position比当前最小的节点还小，那么插入一个最小的节点并更新root
        // 否则，在中间插入一个节点
        auto a_litter_bigger_node = segment_infos.lower_bound(position);
        if(a_litter_bigger_node == segment_infos.end()) {
            a_litter_bigger_node--;
            Node *node = new Node(position, amount, a_litter_bigger_node->second, NULL);
            a_litter_bigger_node->second->next = node;
            segment_infos[position] = node;
            return node;
        }

        if(a_litter_bigger_node->second->position == position) {
            a_litter_bigger_node->second->value += amount;
            return a_litter_bigger_node->second;
        }

        Node *node = new Node(position, amount, NULL, a_litter_bigger_node->second);
        if(a_litter_bigger_node == segment_infos.begin()) {
            // 要往最左边插入
            a_litter_bigger_node->second->prev = node;
            root = node;
        } else {
            // 往中间插入
            node->prev = a_litter_bigger_node->second->prev;
            a_litter_bigger_node->second->prev->next = node;
            a_litter_bigger_node->second->prev = node;
        }
        segment_infos[position] = node;
        return node;
    }

    // 只有值为0，才移除节点
    inline void erase_zero(ll position, Node *node) {
        if(node->value == 0) {
            erase_node(position, node);
        }
    }

    // 时间复杂度o(lgn)
    void erase_node(ll position, Node *node) {
        if(node->prev != NULL) {
            node->prev->next = node->next;
        } else {
            root = node->next;
        }

        if(node->next != NULL) {
            node->next->prev = node->prev;
        }
        segment_infos.erase(position);
        node->next = NULL;
        node->prev = NULL;
        delete node;
    }

    void init(ll from, ll to, ll amount) {
        root = new Node(from, amount, NULL, NULL);
        root->next = new Node(to, -amount, root, NULL);
        segment_infos[from] = root;
        segment_infos[to] = root->next;
    }

    /*
        前缀和算法解决问题：
            segment_infos和root分别用map和链表存储前缀信息
            map<k, Node*>表示从k点开始往后都加node.v，主要用于查找定位快速更新
            root则是线性记录前缀，可o(n)输出结果
    */
    map<ll, Node *> segment_infos;
    Node *root;
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

    segments->set(LLONG_MIN, LLONG_MAX, LLONG_MAX);
    segments->toString();
    if(!segments->verify(map<ll, ll>{{LLONG_MIN, LLONG_MAX}, {LLONG_MAX, -LLONG_MAX}})) {
        cout << "测试全局set失败" << endl;
    } else {
        cout << "测试全局set成功" << endl;
    }

    delete segments;
    return 0;
}