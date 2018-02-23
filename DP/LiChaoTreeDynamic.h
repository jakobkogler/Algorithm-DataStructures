#include <algorithm>
#include <limits>

struct Line {
    long long k, d;

    long long eval(int x) {
        return k * x + d;
    }
};

class LiChaoNode {
public:
    LiChaoNode(Line line) : line(line) {}

    void add_line(Line new_line, int l, int r) {
        int m = (l + r) / 2;
        bool left_smaller = new_line.eval(l) < line.eval(l);
        bool middle_smaller = new_line.eval(m) < line.eval(m);
        if (middle_smaller)
            std::swap(line, new_line);

        if (r - l == 1)
            return;

        if (left_smaller != middle_smaller) {
            if (left == nullptr)
                left = new LiChaoNode(new_line);
            else
                left->add_line(new_line, l, m);
        } else {
            if (right == nullptr)
                right = new LiChaoNode(new_line);
            else
                right->add_line(new_line, m, r);
        }
    }

    long long get_minimum(int x, int l, int r) {
        long long val = line.eval(x);
        int m = (l + r) / 2;
        if (r - l > 1) {
            if (x < m && left != nullptr)
                val = std::min(val, left->get_minimum(x, l, m));
            if (x >= m && right != nullptr)
                    val = std::min(val, right->get_minimum(x, m, r));
        }
        return val;
    }

    void extinguish() {
        if (left != nullptr)
            left->extinguish();
        if (right != nullptr)
            right->extinguish();
        delete this;
    }

private:
    Line line;
    LiChaoNode* left = nullptr;
    LiChaoNode* right = nullptr;
};

class LiChaoTree
{
public:
    LiChaoTree(int mi, int ma) : mi(mi), ma(ma) {
        root = new LiChaoNode({0, std::numeric_limits<long long>::max() / 2});
    }

    ~LiChaoTree() {
        root->extinguish();
    }

    void add_line(Line line) {
        root->add_line(line, mi, ma + 1);
    }

    long long get_minimum(int x) {
        return root->get_minimum(x, mi, ma + 1);
    }

private:
    int mi, ma;
    LiChaoNode* root;
};
