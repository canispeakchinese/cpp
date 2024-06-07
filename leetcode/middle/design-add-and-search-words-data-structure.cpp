#include <iostream>

using std::string;

struct DictTreeNode {
    bool isWord;
    DictTreeNode *children[26];
};

class WordDictionary {
  private:
    DictTreeNode *root;

    void clean(DictTreeNode *root) {
        for(auto &&i : root->children) {
            if(i != nullptr) {
                clean(i);
            }
        }
        delete root;
    }

  public:
    WordDictionary() { root = new DictTreeNode{.isWord = false}; }
    ~WordDictionary() { clean(root); }

    void addWord(string word) {
        auto temp = root;
        for(int i = 0; i < word.size(); ++i) {
            bool isWord = (i == word.size() - 1);
            if(temp->children[word[i] - 'a'] == nullptr) {
                temp->children[word[i] - 'a'] = new DictTreeNode{.isWord = isWord};
            } else if(isWord) {
                temp->children[word[i] - 'a']->isWord = true;
            }
            temp = temp->children[word[i] - 'a'];
        }
    }

    bool search(string word, DictTreeNode *root) {
        for(int i = 0; i < word.size(); i++) {
            if(word[i] == '.') {
                for(int j = 0; j < 26; ++j) {
                    if(root->children[j] != nullptr) {
                        if(search(word.substr(i + 1), root->children[j])) {
                            return true;
                        }
                    }
                }
                return false;
            } else if(root->children[word[i] - 'a'] == nullptr) {
                return false;
            }
            root = root->children[word[i] - 'a'];
        }
        return root->isWord;
    }

    bool search(string word) { return search(word, root); }
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */