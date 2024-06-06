#include <iostream>
#include <map>

using std::map;

struct Node {
    int key;
    Node *frontend;
    Node *backend;
};

class LRUCache {
  private:
    int capacity;
    Node *head, *tail;
    map<int, int> data;
    map<int, Node *> node_map;

    void move2head(int value) {
        Node *node = node_map[value];
        node->frontend->backend = node->backend;
        node->backend->frontend = node->frontend;
        head->backend->frontend = node;
        node->backend = head->backend;
        node->frontend = head;
        head->backend = node;
    }

    void addnode(int value) {
        Node *node = new Node{.key = value};
        node->frontend = head;
        node->backend = head->backend;
        head->backend->frontend = node;
        head->backend = node;
        node_map[value] = node;
    }

    void deletenode(int value) {
        Node *node = node_map[value];
        node->frontend->backend = node->backend;
        node->backend->frontend = node->frontend;
        node_map.erase(value);
        delete node;
    }

  public:
    LRUCache(int capacity) {
        this->capacity = capacity;
        head = new Node();
        tail = new Node();
        head->backend = tail;
        tail->frontend = head;
    }

    ~LRUCache() {
        while(head->backend) {
            head = head->backend;
            delete head->frontend;
        }
        delete head;
    }

    int get(int key) {
        if(data.find(key) == data.end()) {
            return -1;
        }
        move2head(key);
        return data[key];
    }

    void put(int key, int value) {
        if(data.find(key) != data.end()) {
            move2head(key);
            data[key] = value;
            return;
        }
        if(data.size() == capacity) {
            data.erase(tail->frontend->key);
            deletenode(tail->frontend->key);
        }
        addnode(key);
        data[key] = value;
        return;
    }
};

int main(int argc, char *argv[]) {
    LRUCache l(2);
    l.put(1, 1);
    l.put(2, 2);
    l.get(1);
    l.put(3, 3);
    l.get(2);
    l.put(4, 4);
    l.get(1);
    l.get(3);
    l.get(4);
    return 0;
}