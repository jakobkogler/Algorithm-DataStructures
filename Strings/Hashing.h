#include <string>
#include <vector>

class Hashing
{
public:
    Hashing(std::string const& s, int p = 31, int mod = 1'000'000'007) 
      : mod(mod)
    {
        psum.push_back(0);
        long long power = 1;
        long long inv = inverse(p, mod);
        inv_powers.push_back(1);
        for (char c : s) {
            psum.push_back((psum.back() + power * (c - 'a' + 1)) % mod);
            power = power * p % mod;;
            inv_powers.push_back(inv_powers.back() * inv % mod);
        }
    }

    int hash(int i, int j) {
        long long h = psum[j+1] - psum[i];
        if (h < 0)
            h += mod;
        return h * inv_powers[i] % mod;
    }

private:
    long long inverse(long long base, int M) {
        int e = M - 2;
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

    std::vector<int> psum;
    std::vector<int> inv_powers;
    int mod;
};
