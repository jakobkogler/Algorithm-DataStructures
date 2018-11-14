#include <vector>

class Bitset {
public:
    Bitset(int n=0) : data((n + 63)/64) {}
    Bitset(Bitset const& o) : data(o.data) {}

    Bitset& operator^=(Bitset const& o) {
        for (auto i = 0u; i < data.size(); i++)
            data[i] ^= o.data[i];
        return *this;
    }

    bool check(int idx) const {
        return data[idx/64] & (1LL << (idx%64));
    }

    void set(int idx) {
        data[idx/64] |= (1LL << (idx%64));
    }

    int first_set() const {
        for (auto i = 0u; i < data.size(); i++) {
            if (data[i] == 0)
                continue;
            int idx = 64 * i; 
            auto bits = data[i];
            while (!(bits & 1)) {
                idx++;
                bits >>= 1;
            }
            return idx;
        }
        return -1;
    }

private:
    std::vector<long long> data;
};
