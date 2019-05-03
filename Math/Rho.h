#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include "MillerRabin.h"

long long mult(long long a, long long b, long long m) {
    return (__uint128_t)a * b % m;
}

long long f(long long x, long long c, long long m) {
    long long ret = mult(x, x, m) + c;
    if (ret >= m)
        ret -= m;
    return ret;
}

long long brent(long long n, long long x0=2, long long c=1) {
    long long x = x0;
    long long g = 1;
    long long q = 1;
    long long xs, y;

    int m = 128;
    int l = 1;
    while (g == 1) {
        y = x;
        for (int i = 1; i < l; i++)
            x = f(x, c, n);
        int k = 0;
        while (k < l && g == 1) {
            xs = x;
            for (int i = 0; i < m && i < l - k; i++) {
                x = f(x, c, n);
                q = mult(q, std::abs(y - x), n);
            }
            g = std::gcd(q, n);
            k += m;
        }
        l *= 2;
    }
    if (g == n) {
        do {
            xs = f(xs, c, n);
            g = std::gcd(std::abs(xs - y), n);
        } while (g == 1);
    }
    return g;
}

class Factorizer {
public:
    std::vector<long long> factorize(long long x) {
        factors.clear();

        static std::vector<int> primes_100 =
            {2, 3, 5, 7, 11, 13, 17, 19, 23,
             29, 31, 37, 41, 43, 47, 53, 59,
             61, 67, 71, 73, 79, 83, 89, 97};
        for (int p : primes_100) {
            while (x % p == 0) {
                factors.push_back(p);
                x /= p;
            }
        }

        recursive_factorize(x);
        sort(factors.begin(), factors.end());
        return factors;
    }

    void recursive_factorize(long long x) {
        if (x < 100 * 100 || MillerRabin(x)) {
            factors.push_back(x);
        } else {
            int c = 1;
            long long g = x;
            while (g == x) {
                g = brent(x, 2, ++c);
            }
            recursive_factorize(g);
            recursive_factorize(x / g);
        }
    }

private:
    std::vector<long long> factors;
};
