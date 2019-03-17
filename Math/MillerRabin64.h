#include <cstdint>
#include <initializer_list>

using u64 = uint64_t;
using u128 = __uint128_t;

u64 power(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1)
            result = (u128)result * base % mod;
        base = (u128)base * base % mod;
        e >>= 1;
    }
    return result;
}

bool SPRP(u64 n, u64 a, u64 d, int r) {
    u64 x = power(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    for (int s = 1; s < r; s++) {
        x = (u128)x * x % n;
        if (x == n - 1)
            return true;
    }
    return false;
};

bool MillerRabin(u64 n) {
    if (n < 2)
        return false;

    int r = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        r++;
    }

    for (int p : {2, 3, 5, 13, 19, 73, 193, 407521, 299210837}) {
        if (p == n)
            return true;
    }
    for (int a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (!SPRP(n, a, d, r))
            return false;
    }
    return true;
}

bool isPrime(u64 n) {
    for (int p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97}) {
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }
    return MillerRabin(n);
}
