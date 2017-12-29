#include <vector>

int MOD = 1'000'000'007;

long long power(long long base, int e, int M=MOD)
{
    long long result = 1;
    base %= M;
    while (e) {
        if (e & 1)
            result = (result * base) % M;
        base = (base * base) % M;
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
