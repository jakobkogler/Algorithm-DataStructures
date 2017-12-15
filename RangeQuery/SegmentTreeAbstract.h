#include <vector>
#include <limits>

struct Segment {

};

Segment combine(Segment a, Segment b) {
    return a;
}

class SegmentTree {
public:
	SegmentTree(int count) {
		n = count;
		data.resize(2 * n);
	}

	SegmentTree(std::vector<Segment> const &values) {
		n = values.size();
		data.resize(2 * n);
        for (int idx = 0; idx < n; idx++)
            data[idx + n] = values[idx];
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
		Segment ret_l, ret_r;
		left += n;
		right += n;

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
};
