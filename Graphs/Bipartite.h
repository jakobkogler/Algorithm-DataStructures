#include <vector>

std::vector<std::vector<int>> adj;
std::vector<int> side;

bool bipartite(int v, int p = -1, int s = 0)
{
    if (p == -1)
        side.assign(adj.size(), -1);

    if (side[v] >= 0)
        return side[v] == s;
    side[v] = s;

    for (int u : adj[v]) {
        if (u == p)
            continue;
        if (!bipartite(u, v, s ^ 1))
            return false;
    }

    return true;
}
