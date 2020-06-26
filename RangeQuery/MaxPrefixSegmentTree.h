#include <algorithm>
#include <vector>

struct PrefixSum {
    long long sum, prefix_sum;

    PrefixSum operator+(PrefixSum const& other) {
        return {sum + other.sum,
                std::max(prefix_sum, sum + other.prefix_sum)};
    }
};

class MaxPrefixSegmentTree {
public:
    MaxPrefixSegmentTree(int count) {
        n = count;
        prefix_sums.assign(2 * n, {0, 0});
    }

    void update(int idx, int add) {
        idx += n;
        int old_value = prefix_sums[idx].sum;
        int new_value = old_value + add;
        prefix_sums[idx] = {new_value, std::max(0, new_value)};
        while (idx > 1) {
            idx /= 2;
            prefix_sums[idx] = prefix_sums[2 * idx] + prefix_sums[2 * idx + 1];
        }
    }

    long long maximum(int left, int right) {  // interval [left, right)
        left += n;
        right += n;

        std::vector<PrefixSum> from_left, from_right;
        while (left < right) {
            if (left & 1)
                from_left.push_back(prefix_sums[left++]);
            if (right & 1)
                from_right.push_back(prefix_sums[--right]);
            left >>= 1;
            right >>= 1;
        }

        while (!from_right.empty()) {
            from_left.push_back(from_right.back());
            from_right.pop_back();
        }
        long long best = 0;
        PrefixSum cur{0, 0};
        for (auto const& ps : from_left) {
            cur = cur + ps;
            best = std::max(best, cur.prefix_sum);
        }
        return best;
    }

private:
    int n;
    std::vector<PrefixSum> prefix_sums;
};
