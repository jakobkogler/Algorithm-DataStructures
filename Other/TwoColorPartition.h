#include <set>

struct Interval
{
    int l, r, color;
    bool operator<(const Interval& o) const {
        return l < o.l;
    }
};

class TwoColorPartition
{
public:
    TwoColorPartition(int n) {
        s.insert({0, n-1, 0});
    }

    using It = std::set<Interval>::iterator;

    void split(int x) {
        // x becomes the left border of interval
        auto it = s.upper_bound({x, 0, 0});
        --it;
        if (it->l < x) {
            auto I = *it;
            s.erase(it);
            s.insert({I.l, x-1, I.color});
            s.insert({x, I.r, I.color});
        }
    }

    std::pair<It, It> get_iterators(int l, int r) {
        split(l);
        split(r+1);
        return {s.lower_bound({l, 0, 0}), s.upper_bound({r, 0, 0})};
    }

    void color(It lit, It rit, int c) {
        int l = lit->l;
        auto tmp = rit;
        --tmp;
        int r = tmp->r;
        s.erase(lit, rit);
        s.insert({l, r, c});
    }

private:
    std::set<Interval> s;
};
