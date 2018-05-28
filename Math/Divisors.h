#include <vector>
#include <numeric>

std::vector<int> largest;

void computePrimesLargest(int n) {
    largest.resize(n + 1);
    std::iota(largest.begin(), largest.end(), 0);

    for (int i = 2; i <= n; i++) {
        if (i == largest[i]) {
            for (int j = 2 * i; j <= n; j += i) {
                largest[j] = i;
            }
        }
    }
}

std::vector<int> divisors(int x) {
    std::vector<int> d;
    d.push_back(1);

    while (x > 1) {
        int p = largest[x];
        int c = 0;
        while (largest[x] == p) {
            c++;
            x /= p;
        }
        std::vector<int> d2;
        int b = 1;
        for (int j = 0; j <= c; j++) {
            for (int x : d) {
                d2.push_back(x * b);
            }
            b *= p;
        }
        d.swap(d2);
    }
    return d;
}
