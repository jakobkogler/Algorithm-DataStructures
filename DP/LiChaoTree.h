#include <vector>
#include <limits>

struct Line {
    long long k, d;

    long long eval(int x) {
        return k * x + d;
    }
};

class LiChaoTree {
public:
    LiChaoTree(int n) : n(n) {
        inf = {0LL, std::numeric_limits<long long>::max() / 2};
        lines.resize(4 * n);
        reset();
    }

    void reset() {
        fill(lines.begin(), lines.end(), inf);
    }

    void add_line(Line new_line, int id=1, int l=0, int r=-1) {
        if (r == -1)
            r = n;

        int m = (l + r) / 2;
        bool left = new_line.eval(l) < lines[id].eval(l);
        bool middle = new_line.eval(m) < lines[id].eval(m);
        if (middle)
            std::swap(lines[id], new_line);

        if (r - l == 1)
            return;

        if (left != middle)
            add_line(new_line, id << 1, l, m);
        else
            add_line(new_line, id << 1 | 1, m, r);
    }

    long long get_minimum(int x, int id=1, int l=0, int r=-1) {
        if (r == -1)
            r = n;

        long long seg_value = lines[id].eval(x);
        if (r - l == 1)
            return seg_value;

        int m = (l + r) / 2;
        if (x < m)
            return std::min(seg_value, get_minimum(x, id << 1, l, m));
        else 
            return std::min(seg_value, get_minimum(x, id << 1 | 1, m, r));
    }

    int n;
    std::vector<Line> lines;
    Line inf;
};
