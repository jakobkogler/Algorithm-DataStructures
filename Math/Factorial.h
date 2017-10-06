#include <vector>

int MOD = 1'000'000'009;
std::vector<long long> factorial;

void precompute_factorial()
{
    factorial.push_back(1);
    for (int i = 1; i <= 5000; i++)
        factorial.push_back((factorial.back() * i) % MOD);
}
