#include <vector>
#include <iostream>


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

	int prefix_query(int idx) {
		// Computes prefix sum of up to including the idx-th element
		int result = 0;
		for (++idx; idx > 0; idx -= idx & -idx) {
			result += m_array[idx];
		}
		return result;
	}

	int range_query(int from_idx, int to_idx) {
		// Computes the range sum between two indices (both inclusive)
		return prefix_query(to_idx) - prefix_query(from_idx - 1);
	}

	void update(int idx, int add) {
		// Add a value to the idx-th element
		for (++idx; idx < m_array.size(); idx += idx & -idx) {
			m_array[idx] += add;
		}
	}

private:
	std::vector<int> m_array;
};


int main()
{
	std::vector<int> array{ 1, 7, 3, 0, 5, 8, 3, 2, 6, 2, 1, 1, 4, 5 };
	BIT bit(array);
	std::cout << "Array: ";
	for (int val : array) {
		std::cout << val << " ";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Prefix sum of first 13 elements: " << bit.prefix_query(12) << std::endl;
	std::cout << "Prefix sum of first 7 elements: " << bit.prefix_query(6) << std::endl;
	std::cout << "Range sum from pos 1 to pos 5: " << bit.range_query(1, 5) << std::endl;
	std::cout << std::endl;

	bit.update(4, 2);
	std::cout << "Add 2 to element at pos 4" << std::endl;
	std::vector<int> new_array;
	for (int idx = 0; idx < array.size(); idx++) {
		new_array.push_back(bit.range_query(idx, idx));
	}
	std::cout << "Array: ";
	for (int val : array) {
		std::cout << val << " ";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Prefix sum of first 13 elements: " << bit.prefix_query(12) << std::endl;
	std::cout << "Prefix sum of first 7 elements: " << bit.prefix_query(6) << std::endl;
	std::cout << "Range sum from pos 1 to pos 5: " << bit.range_query(1, 5) << std::endl;
	std::cout << std::endl;
}						