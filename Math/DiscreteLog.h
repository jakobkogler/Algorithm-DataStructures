#include <cmath>
#include <numeric>
#include <unordered_map>
#include "Exponentiation.h"

/**
 * minual x for a^x equiv b mod m
 */
int discrete_log(int a, int b, int m) {
    a %= m;
    b %= m;
    if (a == 0) // assuming that 0^0 is undefined
        return b == 0 ? 1 : -1;

    long long k = 1;
    int offset = 0, g;
    while ((g = std::gcd(a, m)) > 1) {
        if (b == k)
            return offset;
        if (b % g)
            return -1;
        b /= g;
        m /= g;
        offset++;
        k = (k * a / g) % m;
    }

    std::unordered_map<int, int> baby;
    int n = std::sqrt(m) + 1;

    // baby steps
    long long cur = b;
    for (int q = 0; q <= n; q++) {
        baby[cur] = q;
        cur = (cur * a) % m;
    }

    // giant steps
    int an = power(a, n, m);
    cur = k;
    for (int p = 1; p <= n; p++) {
        cur = (cur * an) % m;
        if (baby.count(cur))
            return n * p - baby[cur] + offset;
    }
    return -1;
}
