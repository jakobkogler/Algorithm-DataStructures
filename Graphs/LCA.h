#include "../RangeQuery/SparseTable.h"

class LCA
{
public:
    LCA(std::vector<std::vector<int>> adj) : adj(adj) {
        int nodes = adj.size();
        first_encounter.resize(nodes);
        dfs_euler_tour(0, -1);
        st = new SparseTable(euler_tour);
    }

    ~LCA() {
        delete st;
    }

    int query(int u, int v) {
        int fe1 = first_encounter[u];
        int fe2 = first_encounter[v];
        if (fe1 > fe2)
            std::swap(fe1, fe2);
        int LCA_new_index = st->minimum(fe1, fe2+1);
        int LCA_old_index = new_to_old[LCA_new_index];
        return LCA_old_index;
    }

private:
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

    std::vector<std::vector<int>> adj;
    std::vector<int> euler_tour;
    std::vector<int> first_encounter;
    std::vector<int> new_to_old;
    SparseTable* st;
};
