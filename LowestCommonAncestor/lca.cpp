#include <iostream>
#include <vector>

using namespace std;

class RMQ
{
public:
    RMQ(vector<int> v) {
        log_table.assign(v.size() + 1, 0);
        for (int i = 2; i < log_table.size(); i++) 
            log_table[i] = log_table[i/2] + 1;

        sparse_table.assign(log_table.back() + 1, vector<int>(v.size()));
        sparse_table[0] = v;
        for (int row = 1; row < sparse_table.size(); row++) {
            for (int i = 0; i + (1 << row) <= v.size(); i++) {
                sparse_table[row][i] = min(sparse_table[row-1][i], sparse_table[row-1][i+(1<<(row-1))]);
            }
        }
    }

    int minimum(int l, int r) {
        int log = log_table[r - l];
        return min(sparse_table[log][l], sparse_table[log][r - (1 << log)]);
    }

private:
    vector<int> log_table;
    vector<vector<int>> sparse_table;
};

vector<vector<int>> adj;
vector<int> euler_tour;
vector<int> first_encounter;
vector<int> new_to_old;

void dfs_euler_tour(int v, int p) {
    int new_index = new_to_old.size();
    new_to_old.push_back(v);
    first_encounter[v] = euler_tour.size();
    euler_tour.push_back(new_index);

    for (int u : adj[v]) {
        if (u == p)
            continue;

        dfs_euler_tour(u, v);
        euler_tour.push_back(new_index);
    }
}

int main() {
    int nodes = 10;
    vector<pair<int, int>> edges = {{0, 5}, {5, 7}, {7, 2}, {5, 1}, {1, 3}, {1, 6}, {5, 8}, {0, 9}, {9, 4}};

    adj.resize(nodes);
    for (auto edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    first_encounter.resize(nodes);
    dfs_euler_tour(0, -1);
    RMQ rmq(euler_tour);

    int u, v;
    cin >> u >> v;
    int fe1 = first_encounter[u];
    int fe2 = first_encounter[v];
    if (fe1 > fe2)
        swap(fe1, fe2);
    int LCA_new_index = rmq.minimum(fe1, fe2+1);
    int LCA_old_index = new_to_old[LCA_new_index];
    cout << "LCA_new_index: " << LCA_new_index << endl;
    cout << "LCA_old_index: " << LCA_old_index << endl;
}
