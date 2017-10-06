#include <vector>

int MOD = 1'000'000'009;
std::vector<long long> factorial;

void precompute_factorial(int ma)
{
    factorial.push_back(1);
    for (int i = 1; i <= ma; i++)
        factorial.push_back((factorial.back() * i) % MOD);
}
