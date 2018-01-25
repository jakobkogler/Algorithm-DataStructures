#include <vector>
#include <queue>
#include <limits>

// maximal matching in O(sqrt(V) * E)
class HopcroftKarp
{
public:
    HopcroftKarp(int n1, int n2) : n1(n1), n2(n2) {
        adj.resize(n1 + 1);
        match.assign(n1 + 1, 0);
        matched.assign(n2 + 1, 0);
        dist.resize(n1 + 1);
    }

    // nodes are 1-indexed
    void add(int u, int v) {
        adj[u].push_back(v);
    }

    int maximalMatching() {
        int matchings = 0;
        while (bfs()) {
            for (int u = 1; u <= n1; u++) {
                if (!match[u]) {
                    if (dfs(u))
                        matchings++;
                }
            }
        }
        return matchings;
    }

private:
    bool bfs() {
        std::queue<int> q;
        dist[0] = INF;
        for (int u = 1; u <= n1; u++) {
            if (!match[u]) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    if (dist[matched[v]] == INF) {
                        dist[matched[v]] = dist[u] + 1;
                        q.push(matched[v]);
                    }
                }

            }
        }
        return dist[0] != INF;
    }

    bool dfs(int u) {
        if (u == 0)
            return true;

        for (int v : adj[u]) {
            if (dist[matched[v]] == dist[u] + 1) {
                if (dfs(matched[v])) {
                    matched[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INF;
        return false;
    }

    static const int INF = std::numeric_limits<int>::max();
    int n1, n2;
    std::vector<int> dist;
    std::vector<std::vector<int>> adj;
public:
    std::vector<int> match, matched;
};
