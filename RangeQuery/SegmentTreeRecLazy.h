#include <limits>
#include <vector>

class SegmentTreeRecLazy
{
public:
    SegmentTreeRecLazy(int n) : n(n)
    {
        data.assign(4 * n, 0);
        todo.assign(4 * n, 0);
    }

    int minimum(int x, int y, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (x >= r || y <= l) {
            return std::numeric_limits<int>::max();
        } else if (x <= l && r <= y) {
            return data[id];
        } else {
            push(id, l, r);
            int m = (l + r) >> 1;
            return std::min(minimum(x, y, id << 1, l, m),
                            minimum(x, y, id << 1 | 1, m, r));
        }
    }

    void update(int x, int y, int addend, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (x >= r || y <= l) {
        } else if (x <= l && r <= y) {
            data[id] += addend;
            todo[id] += addend;
        } else {
            push(id, l, r);
            int m = (l + r) >> 1;
            update(x, y, addend, id << 1, l, m);
            update(x, y, addend, id << 1 | 1, m, r);
            data[id] = std::min(data[id << 1], data[id << 1 | 1]);
        }
    }

private:
    void push(int id, int l, int r)
    {
        data[id << 1] += todo[id];
        todo[id << 1] += todo[id];
        data[id << 1 | 1] += todo[id];
        todo[id << 1 | 1] += todo[id];
        todo[id] = 0;
    }

    int n;
    std::vector<int> data, todo;
};
