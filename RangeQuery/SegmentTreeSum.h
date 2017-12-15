#include <vector>
#include <limits>

class SegmentTree {
public:
	SegmentTree(int count) {
		n = count;
		data.assign(2 * n, 0);
	}

	SegmentTree(std::vector<int> const &values) {
		n = values.size();
		data.resize(2 * n);
		std::copy(values.begin(), values.end(), &data[0] + n);
		for (int idx = n - 1; idx > 0; idx--)
			data[idx] = data[2 * idx] + data[2 * idx + 1];
	}

	void update(int idx, int value) {
		idx += n;
		data[idx] = value;

		while (idx > 1) {
			idx /= 2;
			data[idx] = data[2 * idx] + data[2 * idx + 1];
		}
	}

	int sum(int left, int right) { // interval [left, right)
		int ret = 0;
		left += n;
		right += n;

		while (left < right) {
			if (left & 1) ret += data[left++];
			if (right & 1) ret += data[--right];
			left >>= 1;
			right >>= 1;
		}
		return ret;
	}

private:
	int n;
	std::vector<int> data;
};
