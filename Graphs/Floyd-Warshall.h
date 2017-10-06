#include <vector>
#include <limits>

void Floyd_Warshall(std::vector<std::vector<int>> &cost) {
    int n = cost.size();

    for (int i = 0; i < n; i++) {
        cost[i][i] = 0;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cost[i][j] = std::min(cost[i][j], cost[i][k] + cost[k][j]);
            }
        }
    }
}
