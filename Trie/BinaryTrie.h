struct BinaryTrie {
    BinaryTrie() {
        ch[0] = ch[1] = nullptr;
        count = 0;
    }

    void add(int val, int idx=30) {
        count++;
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            if (!ch[c])
                ch[c] = new BinaryTrie();
            ch[c]->add(val, idx - 1);
        }
    }

    bool remove(int val, int idx=30) {
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            bool removed = ch[c] && ch[c]->remove(val, idx - 1);
            count -= removed;
            return removed;
        } else {
            if (count) {
                count--;
                return true;
            } else {
                return false;
            }
        }
    }

    int countSmaller(int val, int idx=30) {
        int cnt = 0;
        if (idx >= 0) {
            int c = (val & (1 << idx)) ? 1 : 0;
            if (c == 1 && ch[0])
                cnt += ch[0]->count;
            if (ch[c])
                cnt += ch[c]->countSmaller(val, idx-1);
        }
        return cnt;
    }

    int count;
    BinaryTrie* ch[2];
};
