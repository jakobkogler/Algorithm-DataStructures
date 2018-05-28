#include <vector>

struct LinearFunktion
{
    long long k = 1, d = 0;
    void compose(LinearFunktion f) {
        k *= f.k;
        d = f.k * d + f.d;
    }
};

class SegmentTreeRecLazy
{
public:
    SegmentTreeRecLazy(int n) : n(n)
    {
        data.assign(4 * n, 0);
        todo.resize(4 * n);
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

    void apply(int x, int y, LinearFunktion f, int id = 1, int l = 0, int r = -1)
    {
        if (r == -1)
            r = n;

        if (x >= r || y <= l) {
        } else if (x <= l && r <= y) {
            data[id] = data[id] * f.k + f.d * (r - l);
            todo[id].compose(f);
        } else {
            push(id, l, r);
            int m = (l + r) >> 1;
            apply(x, y, f, id << 1, l, m);
            apply(x, y, f, id << 1 | 1, m, r);
            data[id] = data[id << 1] + data[id << 1 | 1];
        }
    }

private:
    void push(int id, int l, int r)
    {
        int m = (l + r) >> 1;

        data[id << 1] *= todo[id].k;
        data[id << 1] += todo[id].d * (m - l);
        todo[id << 1].compose(todo[id]);

        data[id << 1 | 1] *= todo[id].k;
        data[id << 1 | 1] += todo[id].d * (r - m);
        todo[id << 1 | 1].compose(todo[id]);

        todo[id] = {1, 0};
    }

    int n;
    std::vector<long long> data;
    std::vector<LinearFunktion> todo;
};
