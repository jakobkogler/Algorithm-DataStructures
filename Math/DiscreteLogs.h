#include <cmath>
#include <numeric>
#include <unordered_map>
#include <vector>
#include "Exponentiation.h"

/**
 * minual x for a^x equiv b mod m for every b in [0, bound)
 * assumes m is prime
 */
std::vector<int> discrete_logs(int a, int m, int bound) {
    std::vector<int> logs(bound);
    a %= m;
    logs[0] = a == 0 ? 1 : -1;
    logs[1] = a == 0 ? -1 : 0;

    int n = std::sqrt(m / bound) + 1;
    int m_n = m / n + 1;

    // giant steps
    int an = power(a, n, m);
    long long cur = 1;
    std::unordered_map<int, int> giant;
    for (int p = 1; p <= m_n; p++) {
        cur = (cur * an) % m;
        if (giant.count(cur) == 0)
            giant[cur] = p;
    }
    
    for (int b = 2; b < bound; b++) {
        // baby steps
        int best = m;
        cur = b;
        for (int q = 0; q <= n; q++) {
            if (giant.count(cur)) {
                int p = giant[cur];
                best = std::min(best, n * p - q);
            }
            cur = (cur * a) % m;
        }
        logs[b] = best == m ? -1 : best;
    }
    return logs;
}
