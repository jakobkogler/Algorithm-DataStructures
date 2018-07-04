#include <string>
#include <deque>

struct StringTrieNode;

std::deque<StringTrieNode> cache;

StringTrieNode* fetch_new() {
    cache.emplace_back();
    return &cache.back();
}

StringTrieNode* fetch_new(StringTrieNode const& node) {
    cache.emplace_back(node);
    return &cache.back();
}

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
        auto neww = fetch_new(*this);
        if (idx == (int)s.size()) {
            neww->data = priority;
        } else {
            int c = s[idx] - 'a';
            if (!ch[c])
                neww->ch[c] = fetch_new();
            neww->ch[c] = neww->ch[c]->add(s, priority, idx + 1);
        }
        return neww;
    }

    StringTrieNode* remove(std::string const& s, int idx=0) {
        auto neww = fetch_new(*this);
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
