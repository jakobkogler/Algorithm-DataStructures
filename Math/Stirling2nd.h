#include "FastBinomialCoefficient.h"

ModInt Stirling2nd(int n, int k) {
    ModInt res;
    for (int j = 0; j <= k; j++) {
        ModInt x = Binomial(k, j) * power(ModInt(j), n);
        if ((k - j) & 1)
            res -= x;
        else
            res += x;
    }
    return res * f_inv[k];
}
