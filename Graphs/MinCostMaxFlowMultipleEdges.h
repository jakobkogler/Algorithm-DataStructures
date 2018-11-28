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
        directed_edges.resize(2 * m);
        adj.resize(n);
        for (int i = 0; i < m; i++) {
            Edge e = edges[i];
            directed_edges[2*i] = e;
            directed_edges[2*i+1] = Edge(e.to, e.from, -e.cost, 0);
            adj[e.from].push_back(2*i);
            adj[e.to].push_back(2*i+1);
        }

        auto [negative_cycle, distances] = BellmanFord(directed_edges, source, n);
        assert(!negative_cycle);
        potential = distances;

        int flow = 0;
        int total_cost = 0;
        while (true) {
            auto [distances, prev_edge] = dijkstra(source);
            if (prev_edge[sink] == -1)
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
                int prev_edge_idx = prev_edge[cur];
                Edge& prev_edge = directed_edges[prev_edge_idx];
                cur_flow = std::min(cur_flow, prev_edge.capacity);
                cur = prev_edge.from;
            }
            cur = sink;
            flow += cur_flow;
            while (cur != source) {
                int prev_edge_idx = prev_edge[cur];
                Edge& prev_edge = directed_edges[prev_edge_idx];
                Edge& prev_edge_rev = directed_edges[prev_edge_idx ^ 1];
                total_cost += cur_flow * prev_edge.cost;
                prev_edge.capacity -= cur_flow;
                prev_edge_rev.capacity += cur_flow;
                cur = prev_edge.from;
            }
        }

        return {flow, total_cost};
    }

    std::pair<std::vector<int>, std::vector<int>> dijkstra(int v0) {
        int n = adj.size();
        int INF = std::numeric_limits<int>::max();
        std::vector<int> distance(n, INF), prev_edge(n, -1);
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
            for (auto e_idx : adj[v]) {
                Edge& e = directed_edges[e_idx];
                if (e.capacity == 0)
                    continue;
                int new_cost = e.cost + potential[v]- potential[e.to];
                if (dist + new_cost < distance[e.to]) {
                    distance[e.to] = dist + new_cost;
                    prev_edge[e.to] = e_idx;
                    pq.push({-distance[e.to], e.to});
                }
            }
        }
        return {distance, prev_edge};
    }

private:
    std::vector<int> potential;
    std::vector<std::vector<int>> adj;
    std::vector<Edge> directed_edges;
};
