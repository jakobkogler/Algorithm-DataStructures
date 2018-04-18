#include <algorithm>

class Node {
public:
    Node(int x) : x(x) {}

    void update(int l, int r, int pos, int val) {
        if (l + 1 == r) {
            x = val;
        } else {
            int m = (l + r) / 2;
            if (pos < m) {
                if (left == nullptr)
                    left = new Node(0);
                left->update(l, m, pos, val);
            } else {
                if (right == nullptr)
                    right = new Node(0);
                right->update(m, r, pos, val);
            }
            x = 0;
            if (left)
                x = std::max(x, left->x);
            if (right)
                x = std::max(x, right->x);
        }
    }

    int maximum(int l, int r, int rl, int rr) {
        if (rr <= l || rl >= r)
            return 0;
        if (rl <= l && r <= rr)
            return x;
        int m = (l + r) / 2;
        int res = 0;
        if (left)
            res = std::max(res, left->maximum(l, m, rl, rr));
        if (right)
            res = std::max(res, right->maximum(m, r, rl, rr));
        return res;
    }

    int x;
    Node* left = nullptr;
    Node* right = nullptr;
};

class ImplicitSegmentTree {
public:
    ImplicitSegmentTree(int n) : n(n), root(new Node(0)) {}

    void update(int pos, int val) {
        root->update(0, n, pos, val);
    }

    int maximum(int l, int r) {
        return root->maximum(0, n, l, r);
    }

private:
    int n;
    Node* root = nullptr;
};
