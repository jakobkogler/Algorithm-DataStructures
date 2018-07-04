#include <deque>
#include <limits>

struct Node;

std::deque<Node> cache;

Node* fetch_new() {
    cache.emplace_back();
    return &cache.back();
}

Node* fetch_new(Node const& node) {
    cache.emplace_back(node);
    return &cache.back();
}

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int data = std::numeric_limits<int>::max();

    void build(int l, int r) {
        if (l + 1 < r) {
            int m = (l + r) / 2;
            left = fetch_new();
            right = fetch_new();
            left->build(l, m);
            right->build(m, r);
        }
    }

    Node* set(int pos, int x, int l, int r) {
        Node* neww = fetch_new(*this);

        if (l + 1 == r) {
            neww->data = x;
        } else {
            int m = (l + r) / 2;
            if (pos < m)
                neww->left = neww->left->set(pos, x, l, m);
            else
                neww->right = neww->right->set(pos, x, m, r);
            neww->data = std::min(neww->left->data, neww->right->data);
        }
        return neww;
    }

    int min(int bl, int br, int l, int r) {
        if (bl >= r || br <= l)
            return std::numeric_limits<int>::max();

        if (bl <= l && r <= br)
            return data;

        int m = (l + r) / 2;
        return std::min(left->min(bl, br, l, m), right->min(bl, br, m, r));
    }
};
