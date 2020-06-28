#include <numeric>
#include <vector>

class UnionFind
{
public:
    UnionFind(int n)
    {
        p.resize(n);
        std::iota(p.begin(), p.end(), 0);
        sets.resize(n);
        for (int i = 0; i < n; i++)
            sets[i].push_back(i);
    }

    int Find(int x) { return p[x]; }

    bool Union(int x, int y)
    {
        int xRoot = Find(x);
        int yRoot = Find(y);

        if (xRoot == yRoot)
            return false;

        if (sets[xRoot].size() < sets[yRoot].size())
            swap(xRoot, yRoot);

        for (int elem : sets[yRoot]) {
            p[elem] = xRoot;
            sets[xRoot].push_back(elem);
        }
        sets[yRoot].clear();

        return true;
    }

    bool sameUnion(int x, int y) { return Find(x) == Find(y); }

private:
    std::vector<int> p;
    std::vector<std::vector<int>> sets;
};
