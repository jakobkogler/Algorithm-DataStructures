#include <vector>
#include <queue>

std::vector<std::vector<int>> adj;

std::vector<int> bfs(int start) {
    int n = adj.size();
    std::vector<bool> found(n, false);
    std::queue<std::pair<int, int>> q;
    std::vector<int> dist_to_start(n);

    q.push({start, 0});
    found[start] = true;

    while (!q.empty()) {
        auto p = q.front();
        int cur = p.first;
        int dist = p.second;
        q.pop();

        dist_to_start[cur] = dist;

        for (auto next : adj[cur]) {
            if (!found[next]) {
                q.push({next, dist + 1});
                found[next] = true;
            }
        }
    }

    return dist_to_start;
}
