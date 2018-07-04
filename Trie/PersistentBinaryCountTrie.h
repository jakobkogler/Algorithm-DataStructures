#include <deque>

struct BinaryCountTrie;

std::deque<BinaryCountTrie> cache;

BinaryCountTrie* fetch_new() {
    cache.emplace_back();
    return &cache.back();
}

BinaryCountTrie* fetch_new(BinaryCountTrie const& node) {
    cache.emplace_back(node);
    return &cache.back();
}

struct BinaryCountTrie {
    BinaryCountTrie() {
        for (int i = 0; i < 2; i++) {
            ch[i] = nullptr;
        }
        count = 0;
    }

    BinaryCountTrie(BinaryCountTrie* old) {
        for (int i = 0; i < 2; i++) {
            ch[i] = old->ch[i];
        }
        count = old->count;
    }

    BinaryCountTrie* add(int val, int idx=30) {
        auto neww = fetch_new(*this);
        neww->count++;
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            if (!neww->ch[c])
                neww->ch[c] = fetch_new();
            neww->ch[c] = neww->ch[c]->add(val, idx - 1);
        }
        return neww;
    }

    BinaryCountTrie* remove(int val, int idx=30) {
        auto neww = fetch_new(*this);
        neww->count--;
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            if (neww->ch[c])
                neww->ch[c] = neww->ch[c]->remove(val, idx - 1);
        }
        return neww;
    }

    int countSmaller(int val, int idx=30) {
        int cnt = 0;
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            for (int i = 0; i < c; i++) {
                if (ch[i])
                    cnt += ch[i]->count;
            }
            if (ch[c])
                cnt += ch[c]->countSmaller(val, idx-1);
        }
        return cnt;
    }

    int count;
    BinaryCountTrie* ch[2];
};
