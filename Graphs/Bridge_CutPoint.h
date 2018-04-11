#include <vector>

struct Edge {
    int to;
    int idx;
    Edge(int to, int idx) : to(to), idx(idx) {}
};

int n, m;
std::vector<std::vector<Edge>> adj;

int timer;
std::vector<int> disc, low;
std::vector<bool> bridge, cut;

void dfs(int v, int p=-1) {
    disc[v] = low[v] = ++timer;
    int children = 0;
    for (Edge e : adj[v]) {
        int u = e.to;
        if (u == p)
            continue;

        if (disc[u]) {
            low[v] = std::min(low[v], disc[u]);
        } else {
            children++;
            dfs(u, v);
            low[v] = std::min(low[v], low[u]);
            if (low[u] > disc[v])
                bridge[e.idx] = true;
            if ((p != -1 && low[u] >= disc[v]) ||
                (p == -1 && children > 1))
                cut[v] = true;
        }
    }
}

void find() {
    timer = 0;
    disc.assign(n, 0);
    low.assign(n, 0);
    bridge.assign(m, false);
    cut.assign(n, false);

    for (int v = 0; v < n; v++) {
        if (!disc[v])
            dfs(v);
    }
}
