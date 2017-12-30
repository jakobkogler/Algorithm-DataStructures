#include <vector>
#include <limits>
#include <functional>

template<typename Segment>
class SegmentTree {
public:
    using CombineFkt = std::function<Segment(Segment, Segment)>;

    SegmentTree(int count, CombineFkt combine) : combine{combine} {
        n = count;
        data.resize(2 * n);
    }

    SegmentTree(std::vector<Segment> const &values, CombineFkt combine) : combine{combine} {
        n = values.size();
        data.resize(2 * n);
        std::copy(values.begin(), values.end(), &data[n]);
        for (int idx = n - 1; idx > 0; idx--)
            data[idx] = combine(data[idx * 2], data[idx * 2 + 1]);
    }

    void update(int idx, Segment value) {
        idx += n;
        data[idx] = value;

        while (idx > 1) {
            idx /= 2;
            data[idx] = combine(data[2 * idx], data[2 * idx + 1]);
        }
    }

    Segment minimum(int left, int right) { // interval [left, right)
        left += n;
        right += n;

        if (left + 1 == right)
            return data[left];

        Segment ret_l = data[left++], ret_r = data[--right];

        while (left < right) {
            if (left & 1) ret_l = combine(ret_l, data[left++]);
            if (right & 1) ret_r = combine(data[--right], ret_r);
            left >>= 1;
            right >>= 1;
        }
        return combine(ret_l, ret_r);
    }

private:
    int n;
    std::vector<Segment> data;
    CombineFkt combine;
};
