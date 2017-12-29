#include <vector>

struct Edge {
    int from, to, idx;
};

std::vector<std::vector<Edge>> adj;

std::vector<int> color;
std::vector<Edge> odd_cycle;
std::vector<Edge> pre;

bool findOddCycle(Edge e = {-1, 0, -1}, int c = 0)
{
    int v = e.to;
    int p = e.from;

    if (p == -1) {
        color.assign(adj.size(), -1);
        pre.resize(adj.size());
    }

    if (color[v] >= 0) {
        // even cycle
        if (color[v] == c)
            return false;

        // odd cycle
        odd_cycle.push_back(e);
        int cur = p;
        do {
            odd_cycle.push_back(pre[cur]);
            cur = pre[cur].to;
        } while (cur != v);
        return true;
    }

    color[v] = c;
    pre[v] = e;

    for (auto f : adj[v]) {
        if (f.idx == e.idx)
            continue;
        if (findOddCycle(f, c ^ 1))
            return true;
    }

    return false;
}
