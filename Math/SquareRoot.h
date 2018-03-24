#include <limits>

int sqrt(long long x) {
    long long L = 0;
    long long R = std::numeric_limits<int>::max();
    while (L + 1 < R) {
        long long M = (L + R) / 2;
        if (M * M <= x)
            L = M;
        else
            R = M;
    }
    return L;
}
