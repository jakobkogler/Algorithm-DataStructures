#include <algorithm>
#include <vector>

class SCC
{
public:
    SCC(int n) : n(n) {
        adj.resize(n);
        adj_t.resize(n);
    }

    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        adj_t[v].push_back(u);
    }

    int solve()
    {
        visited.assign(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i])
                dfs1(i);
        }
        visited.assign(n, false);
        reverse(order.begin(), order.end());
        components.clear();
        component.resize(n);
        int c = 0;
        for (int i : order) {
            if (!visited[i]) {
                components.push_back({});
                dfs2(i, c);
                c++;
            }
        }

        adj_condensation.resize(c);
        for (int v = 0; v < n; v++) {
            for (int u : adj[v]) {
                if (component[v] != component[u])
                    adj_condensation[component[v]].push_back(component[u]);
            }
        }
        for (int v = 0; v < c; v++) {
            auto& vec = adj_condensation[v];
            sort(vec.begin(), vec.end());
            vec.resize(unique(vec.begin(), vec.end()) - vec.begin());
        }

        return c;
    }

    void dfs1(int v)
    {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u])
                dfs1(u);
        }
        order.push_back(v);
    }

    void dfs2(int v, int c)
    {
        visited[v] = true;
        components.back().push_back(v);
        component[v] = c;
        for (int u : adj_t[v]) {
            if (!visited[u])
                dfs2(u, c);
        }
    }

    int n;
    std::vector<std::vector<int>> adj, adj_t, components, adj_condensation;
    std::vector<bool> visited;
    std::vector<int> order, component;
};
