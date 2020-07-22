#include <vector>
#include <algorithm>

namespace TopologicalSort {
    using AdjacencyList = std::vector<std::vector<int>>;
    std::vector<bool> visited;
    std::vector<int> order;

    void dfs(int v, AdjacencyList const& adj) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u])
                dfs(u, adj);
        }
        order.push_back(v);
    }

    std::vector<int> sort(AdjacencyList const& adj) {
        int n = adj.size();
        order.clear();
        order.reserve(n);
        visited.assign(n, false);
        for (int v = 0; v < n; v++) {
            if (!visited[v])
                dfs(v, adj);
        }
        std::reverse(order.begin(), order.end());
        return order;
    }
}
