#include <vector>

int MOD = 1'000'000'007;

auto power(long long base, int e)
{
    long long result = 1;
    while (e) {
        if (e & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1;
    }
    return result;
}

auto inverse(long long x) {
    return power(x, MOD - 2);
}

auto inverse(std::vector<long long> v) {
    for (auto& x : v)
        x = inverse(x);
    return v;
}
