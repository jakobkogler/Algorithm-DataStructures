#include <vector>
#include <queue>
#include <limits>
#include <tuple>

class EdmondKarp
{
public:
    EdmondKarp(int n, std::vector<std::vector<int>> capacity, std::vector<std::vector<int>> adj)
        : n(n), capacity(capacity), adj(adj) {}
 
    int maxflow(int s, int t) {
        int flow = 0;
 
        std::vector<int> parent(n);
        int new_flow;
 
        while ((new_flow = bfs(s, t, parent))) {
            flow += new_flow;
            int node = t;
            while (node != s) {
                int prev = parent[node];
                capacity[prev][node] -= new_flow;
                capacity[node][prev] += new_flow;
                node = prev;
            }
        }
 
        return flow;
    }
 
    int bfs(int s, int t, std::vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        std::queue<std::pair<int, int>> q;
        q.push({s, std::numeric_limits<int>::max()});
 
        while (!q.empty()) {
            int node, flow;
            std::tie(node, flow) = q.front();
            q.pop();
 
            for (auto next : adj[node]) {
                if (parent[next] == -1 && capacity[node][next]) {
                    parent[next] = node;
                    int new_flow = std::min(flow, capacity[node][next]);
                    if (next == t)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
 
        return 0;
    }
 
    int n;
    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> adj;
};



