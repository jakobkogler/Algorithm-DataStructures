#include <string>

struct StringTrieNode {
    StringTrieNode() {
        for (int i = 0; i < 26; i++) {
            ch[i] = nullptr;
        }
        data = -1;
    }

    StringTrieNode(StringTrieNode* old) {
        for (int i = 0; i < 26; i++) {
            ch[i] = old->ch[i];
        }
        data = old->data;
    }

    StringTrieNode* add(std::string const& s, int priority, int idx=0) {
        auto neww = new StringTrieNode(this);
        if (idx == (int)s.size()) {
            neww->data = priority;
        } else {
            int c = s[idx] - 'a';
            if (!ch[c])
                neww->ch[c] = new StringTrieNode();
            neww->ch[c] = neww->ch[c]->add(s, priority, idx + 1);
        }
        return neww;
    }

    StringTrieNode* remove(std::string const& s, int idx=0) {
        auto neww = new StringTrieNode(this);
        if (idx == (int)s.size()) {
            neww->data = -1;
        } else {
            int c = s[idx] - 'a';
            if (neww->ch[c])
                neww->ch[c] = neww->ch[c]->remove(s, idx + 1);
        }
        return neww;
    }

    int getData(std::string const& s, int idx=0) {
        if (idx == (int)s.size()) {
            return data;
        } else {
            int c = s[idx] - 'a';
            if (ch[c] == nullptr)
                return -1;
            else 
                return ch[c]->getData(s, idx+1);
        }
    }

    int data;
    StringTrieNode* ch[26];
};
