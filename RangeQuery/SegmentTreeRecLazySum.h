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

    SegmentTreeRecLazy(std::vector<long long> const& v) {
        n = v.size();
        data.assign(4 * n, 0);
        todo.assign(4 * n, 0);
        build(v);
    }

    void build(std::vector<long long> const& v, int id = 1, int l = 0, int r = -1) {
        if (r == -1)
            r = n;

        if (l == r - 1) {
            data[id] = v[l];
        } else {
            int m = (l + r) >> 1;
            build(v, id << 1, l, m);
            build(v, id << 1 | 1, m, r);
            data[id] = data[id << 1] + data[id << 1 | 1];
        }
    }

    long long sum(int x, int y, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (x >= r || y <= l) {
            return 0;
        } else if (x <= l && r <= y) {
            return data[id];
        } else {
            push(id, l, r);
            int m = (l + r) >> 1;
            return sum(x, y, id << 1, l, m) + sum(x, y, id << 1 | 1, m, r);
        }
    }

    void add(int x, int y, long long addend, int id = 1, int l = 0, int r = -1)
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
            add(x, y, addend, id << 1, l, m);
            add(x, y, addend, id << 1 | 1, m, r);
            data[id] = std::min(data[id << 1], data[id << 1 | 1]);
        }
    }

private:
    void push(int id, int l, int r)
    {
        int m = (l + r) >> 1;
        data[id << 1] += todo[id] * (m - l);
        todo[id << 1] += todo[id];
        data[id << 1 | 1] += todo[id] * (r - m);
        todo[id << 1 | 1] += todo[id];
        todo[id] = 0;
    }

    int n;
    std::vector<long long> data, todo;
};
