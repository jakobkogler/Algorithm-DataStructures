#include <limits>
#include <vector>

class SegmentTreeRecLazy
{
public:
    SegmentTreeRecLazy(int n) : n(n) { data.assign(4 * n, 0); }

    int minimum(int x, int y, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (x >= r || y <= l) {
            return std::numeric_limits<int>::max();
        } else if (x <= l && r <= y) {
            return data[id];
        } else {
            int m = (l + r) >> 1;
            return std::min(minimum(x, y, id << 1, l, m),
                            minimum(x, y, id << 1 | 1, m, r));
        }
    }

    void update(int pos, int addend, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (pos < l || pos >= r) {
        } else if (l == pos && pos + 1 == r) {
            data[id] += addend;
        } else {
            int m = (l + r) >> 1;
            update(pos, addend, id << 1, l, m);
            update(pos, addend, id << 1 | 1, m, r);
            data[id] = std::min(data[id << 1], data[id << 1 | 1]);
        }
    }

private:
    int n;
    std::vector<int> data;
};
