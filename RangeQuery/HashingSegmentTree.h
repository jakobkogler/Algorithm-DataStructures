#include <vector>

class HashingSegmentTree {
public:
    HashingSegmentTree(int count, int p, int mod) {
        n = count;
        data.assign(2 * n, 0);
        data2.assign(2 * n, 0);
        powers.resize(n + 1);
        powers[0] = 1;
        _mod = mod;
        for (int i = 0; i < n; i++) {
            powers[i+1] = (long long)powers[i] * p % mod;
        }
    }

    void update(int _idx, int value) {
        int idx = _idx + n;
        data[idx] = value * (long long)powers[_idx] % _mod;
        data2[idx] = value;

        while (idx > 1) {
            idx /= 2;
            data[idx] = data[2 * idx] + data[2 * idx + 1];
            if (data[idx] >= _mod)
                data[idx] -= _mod;
            data2[idx] = data2[2 * idx] + data2[2 * idx + 1];
        }
    }

    int sum(int _left, int _right) { // interval [left, right)
        long long ret = 0;
        int left = _left + n;
        int right = _right + n;

        while (left < right) {
            if (left & 1) ret += data[left++];
            if (right & 1) ret += data[--right];
            left >>= 1;
            right >>= 1;
        }

        return ret % _mod * powers[n - _left] % _mod;
    }

private:
    int n;
    int _mod;
    std::vector<int> data, data2;
    std::vector<int> powers;
};
