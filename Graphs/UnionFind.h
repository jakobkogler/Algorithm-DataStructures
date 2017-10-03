#include <algorithm>
#include <vector>

class UnionFind
{
public:
    UnionFind(int n)
    {
        p.resize(n);
        std::iota(p.begin(), p.end(), 0);
        rank.assign(n, 0);
    }

    int Find(int x) { return x == p[x] ? x : p[x] = Find(p[x]); }

    void Union(int x, int y)
    {
        int xRoot = Find(x);
        int yRoot = Find(y);

        if (xRoot == yRoot)
            return;

        if (rank[xRoot] < rank[yRoot]) {
            p[xRoot] = yRoot;
        } else if (rank[xRoot] > rank[yRoot]) {
            p[yRoot] = xRoot;
        } else {
            p[yRoot] = xRoot;
            rank[xRoot]++;
        }
    }

    bool sameUnion(int x, int y) { return Find(x) == Find(y); }

private:
    std::vector<int> p, rank;
};
