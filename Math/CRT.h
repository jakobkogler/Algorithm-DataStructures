#include <vector>
#include "Exponentiation.h"

struct Congruence
{
    long long a, m, totient_m;
};

class CRT
{
public:
    void add_congruence(long long a, long long m, long long totient_m) {
        congruences.emplace_back(a, m, totient_m);
    }

    long long get_unique_solution() {
        long long M = 1;
        for (const auto& c : congruences) {
            M *= c.m;
        }

        long long solution = 0;
        for (const auto& c : congruences) {
            auto b = M / c.m;
            solution = (solution + c.a * b % M * power(b, c.totient_m, c.m)) % M;
        }
        return solution;
    }

    std::vector<Congruence> congruences;
};
