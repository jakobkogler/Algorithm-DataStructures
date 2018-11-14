#include <vector>
#include <math.h>

std::vector<int> compute_primes(int n) {
    const int S = 30000;
    int nsqrt = std::round(std::sqrt(n));

    std::vector<char> is_prime(nsqrt + 1, true);
    std::vector<int> primes, sieve_primes, start_indices;
    for (int i = 3; i <= nsqrt; i += 2) {
        if (is_prime[i]) {
            sieve_primes.push_back(i);
            start_indices.push_back((i * i - 1) / 2);
            for (int j = i * i; j <= nsqrt; j += 2 * i)
                is_prime[j] = false;
        }
    }

    primes.push_back(2);
    std::vector<char> block(S);
    int high = (n - 1) / 2;
    for (int low = 0; low <= high; low += S) {
        fill(block.begin(), block.end(), true);
        for (auto i = 0u; i < sieve_primes.size(); i++) {
            int p = sieve_primes[i];
            int idx = start_indices[i];
            for (; idx < S; idx += p)
                block[idx] = false;
            start_indices[i] = idx - S;
        }
        if (low == 0)
            block[0] = false;
        for (int i = 0; i < S && low + i <= high; i++) {
            if (block[i])
                primes.push_back((low + i) * 2 + 1);
        }
    };

    return primes;
}
