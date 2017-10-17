#include "UnionFind.h"

struct Edge {
    int u, v, w;
    bool used = false;
};

int MST_Kruskal(std::vector<Edge> edges, int nodes)
{
    sort(edges.begin(), edges.end(),
         [](Edge const &e, Edge const &f) { return e.w < f.w; });

    UnionFind uf(nodes);
    int cost = 0;
    for (Edge &e : edges) {
        if (!uf.sameUnion(e.u, e.v)) {
            uf.Union(e.u, e.v);
            e.used = true;
            cost += e.w;
        }
    }
    return cost;
}
