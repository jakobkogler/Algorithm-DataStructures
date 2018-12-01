#include <array>
#include <vector>

template <int C>
class Hashing {
public:
    Hashing(std::string const& s, std::array<std::pair<int, int>, C> pm) {
        n = s.size();
        for (int c = 0; c < C; c++) {
            int p = pm[c].first;
            m[c] = pm[c].second;
            p_pow[c].assign(n, 0);
            p_pow[c][0] = 1;
            for (int i = 1; i < n; i++) {
                p_pow[c][i] = (p_pow[c][i - 1] * p) % m[c];
            }

            h[c].assign(n + 1, 0);
            for (int i = 0; i < n; i++) {
                h[c][i + 1] = (h[c][i] + (s[i] - 'a' + 1) * p_pow[c][i]) % m[c];
            }
        }
    }

    std::array<int, C> hash(int i, int l) const {
        std::array<int, C> hashes;
        for (int c = 0; c < C; c++) {
            long long cur_h = (h[c][i + l] + m[c] - h[c][i]) % m[c];
            hashes[c] = cur_h * p_pow[c][n - i - 1] % m[c];
        }
        return hashes;
    }

    std::array<int, C> default_values() const {
        std::array<int, C> arr;
        std::fill(arr.begin(), arr.end(), -1);
        return arr;
    }

    int n;
    std::array<std::vector<long long>, C> p_pow;
    std::array<std::vector<long long>, C> h;
    std::array<int, C> m;
};

std::array<std::pair<int, int>, 3> pm3 = {{{31, 1'000'000'007}, {37, 1'000'000'007}, {41, 900'000'011}}};
