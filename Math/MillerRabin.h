#include <cstdint>
#include <initializer_list>

using u64 = uint64_t;

u64 power(u64 base, u64 e, u64 mod) {
    u64 result = 1;
    base %= mod;
    while (e) {
        if (e & 1)
            result = result * base % mod;
        base = base * base % mod;
        e >>= 1;
    }
    return result;
}

bool SPRP(u64 n, u64 a, u64 d, int r) {
    u64 x = power(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    for (int s = 1; s < r; s++) {
        x = x * x % n;
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

    for (int a : {2, 3, 5, 7}) {
        if (a == n)
            return true;
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
