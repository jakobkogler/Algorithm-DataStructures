#include <vector>

int MOD = 1'000'000'009;
std::vector<std::vector<long long>> binomial;

void precompute_binomial_coefficients(int ma)
{
    binomial.assign(ma + 1, std::vector<long long>(ma + 1));
    for (int n = 0; n <= ma; n++) {
        binomial[n][0] = 1;
        binomial[n][n] = 1;
        for (int k = 1; k < n; k++) {
            binomial[n][k] =
                (binomial[n - 1][k - 1] + binomial[n - 1][k]) % MOD;
        }
    }
}
