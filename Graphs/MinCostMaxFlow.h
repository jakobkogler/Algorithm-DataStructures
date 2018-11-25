#include <vector>
#include <limits>
#include <cassert>
#include <queue>

struct Edge {
    int from, to, cost, capacity;
    Edge(int from=-1, int to=-1, int cost=0, int capacity=0)
        : from(from), to(to), cost(cost), capacity(capacity) {
    }
};

std::pair<bool, std::vector<int>> BellmanFord(std::vector<Edge> const& edges, int s, int n) {
    int INF = std::numeric_limits<int>::max();
    std::vector<int> d(n, INF);
    d[s] = 0;

    bool improved = true;
    for (int i = 0; i < n && improved; ++i) {
        improved = false;
        for (Edge const& e : edges) {
            if (e.capacity > 0 && d[e.from] < INF && d[e.from] + e.cost < d[e.to]) {
                d[e.to] = d[e.from] + e.cost;
                improved = true;
            }
        }
    }
    return {improved, d};  // true if negative cycle
}

class MinCostMaxFlow {
public:
    std::pair<int, int> solve(std::vector<Edge> const& edges, int source, int sink, int n) {
        int m = (int)edges.size();
        std::vector<Edge> directed_edges(2 * m);
        capacity.assign(n, std::vector<int>(n, 0));
        cost.assign(n, std::vector<int>(n, 0));
        adj.resize(n);
        for (int i = 0; i < m; i++) {
            Edge e = edges[i];
            directed_edges[i] = e;
            directed_edges[i + m] = Edge(e.to, e.from, -e.cost, 0);
            capacity[e.from][e.to] = e.capacity;
            cost[e.from][e.to] = e.cost;
            cost[e.to][e.from] = -e.cost;
            adj[e.from].push_back(e.to);
            adj[e.to].push_back(e.from);
        }

        auto [negative_cycle, distances] = BellmanFord(directed_edges, source, n);
        assert(!negative_cycle);
        potential = distances;

        int flow = 0;
        int total_cost = 0;
        while (true) {
            auto [distances, parent] = dijkstra(source);
            if (parent[sink] == -1)
                break;

            // fix potentials
            for (int i = 0; i < n; i++) {
                if (distances[i] < std::numeric_limits<int>::max())
                    potential[i] += distances[i];
            }

            // augment path
            int cur_flow = std::numeric_limits<int>::max();
            int cur = sink;
            while (cur != source) {
                int prev = parent[cur];
                cur_flow = std::min(cur_flow, capacity[prev][cur]);
                cur = prev;
            }
            cur = sink;
            flow += cur_flow;
            while (cur != source) {
                int prev = parent[cur];
                total_cost += cur_flow * cost[prev][cur];
                capacity[prev][cur] -= cur_flow;
                capacity[cur][prev] += cur_flow;
                cur = prev;
            }
        }

        return {flow, total_cost};
    }

    std::pair<std::vector<int>, std::vector<int>> dijkstra(int v0) {
        int n = adj.size();
        int INF = std::numeric_limits<int>::max();
        std::vector<int> distance(n, INF), parent(n, -1);
        std::priority_queue<std::pair<int, int>> pq;
        pq.push({0, v0});
        distance[v0] = 0;
        while (!pq.empty()) {
            auto x = pq.top();
            pq.pop();
            long long dist = -x.first;
            int v = x.second;
            if (dist > distance[v])
                continue;
            for (auto u : adj[v]) {
                if (capacity[v][u] == 0)
                    continue;
                int new_cost = cost[v][u] + potential[v]- potential[u];
                if (dist + new_cost < distance[u]) {
                    distance[u] = dist + new_cost;
                    parent[u] = v;
                    pq.push({-distance[u], u});
                }
            }
        }
        return {distance, parent};
    }

private:
    std::vector<int> potential;
    std::vector<std::vector<int>> capacity, cost;
    std::vector<std::vector<int>> adj;
};
