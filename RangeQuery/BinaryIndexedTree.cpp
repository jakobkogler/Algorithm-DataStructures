#include <vector>
#include <iostream>
#include <iomanip>


class BIT {
	// Implementation of a Binary Indexed Tree (Fennwick Tree)
public:
	/*BIT(std::vector<int> list) {
		// Initialize BIT with list in O(n*log(n))
		m_array = std::vector<int>(list.size() + 1, 0);
		for (int idx = 0; idx < list.size(); idx++) {
			update(idx, list[idx]);
		}
	}*/

	BIT(std::vector<int> list) {
		// Initialize BIT with list in O(n)
		m_array = std::vector<int>(list.size() + 1, 0);
		for (int idx = 0; idx < list.size(); idx++) {
			m_array[idx + 1] = list[idx];
		}

		for (int idx = 1; idx < m_array.size(); idx++) {
			int idx2 = idx + (idx & -idx);
			if (idx2 < m_array.size()) {
				m_array[idx2] += m_array[idx];
			}
		}
	}

	int prefix_query(int idx) const {
		// Computes prefix sum of up to the element at index idx
		int result = 0;
		for (++idx; idx > 0; idx -= idx & -idx) {
			result += m_array[idx];
		}
		return result;
	}

	int range_query(int from_idx, int to_idx) const {
		// Computes the range sum between two indices (both inclusive)
        if (from_idx == 0)
            return prefix_query(to_idx);
        else
            return prefix_query(to_idx) - prefix_query(from_idx - 1);
	}

	void update(int idx, int add) {
		// Add a value to the element at index idx
		for (++idx; idx < m_array.size(); idx += idx & -idx) {
			m_array[idx] += add;
		}
	}

private:
	std::vector<int> m_array;
};

void print(BIT const& bit, int length) {
	std::cout << "Index: ";
	for (int idx = 0; idx < length; ++idx)
		std::cout << std::setw(2) << idx << " ";
	std::cout << std::endl;
	std::cout << "Array: ";
	for (int idx = 0; idx < length; ++idx)
		std::cout << std::setw(2) << bit.range_query(idx, idx) << " ";
	std::cout << std::endl << std::endl;

	std::cout << "Prefix sum of first 13 elements: \t" << bit.prefix_query(12) << std::endl;
	std::cout << "Prefix sum of first 7 elements: \t" << bit.prefix_query(6) << std::endl;
	std::cout << "Range sum from index 1 to index 5: \t" << bit.range_query(1, 5) << std::endl;
	std::cout << std::endl;
}

int main()
{
	std::vector<int> array{ 1, 7, 3, 0, 5, 8, 3, 2, 6, 2, 1, 1, 4, 5 };
    int length = array.size();
	BIT bit(array);
    print(bit, length);

	bit.update(4, 2);
	std::cout << "Add 2 to element at index 4" << std::endl << std::endl;
    print(bit, length);
}
