#include <vector>

class LCA_Binary_Lifting
{
public:
    LCA_Binary_Lifting(std::vector<std::vector<int>> adj, int root)
        : root(root), adj(adj)
    {
        int n = adj.size();

        log = 1;
        while ((1 << log) < n)
            log++;
        parents.assign(log, std::vector<int>(n, -1));

        height.resize(n);
        dfs(root, -1, 0);

        for (int l = 1; l < log; l++) {
            for (int v = 0; v < n; v++) {
                if (parents[l - 1][v] >= 0)
                    parents[l][v] = parents[l - 1][parents[l - 1][v]];
            }
        }
    }

    int find_parent(int v, int up)
    {
        for (int l = 0; v != -1 && up > 0; l++, up >>= 1) {
            if (up & 1) {
                v = parents[l][v];
            }
        }
        return v;
    }

    int LCA(int u, int v)
    {
        if (height[u] < height[v])
            std::swap(u, v);
        u = find_parent(u, height[u] - height[v]);

        for (int l = log - 1; l >= 0; l--) {
            if (parents[l][u] != parents[l][v]) {
                u = parents[l][u];
                v = parents[l][v];
            }
        }

        if (u == v)
            return u;
        else
            return parents[0][u];
    }

private:
    void dfs(int v, int p, int h)
    {
        parents[0][v] = p;
        height[v] = h;
        for (int u : adj[v]) {
            if (u != p)
                dfs(u, v, h + 1);
        }
    }

    int log;
    int root;
    std::vector<std::vector<int>> adj, parents;
    std::vector<int> height;
};
