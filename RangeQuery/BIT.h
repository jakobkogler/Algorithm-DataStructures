#include <vector>

// Implementation of a Binary Indexed Tree (Fennwick Tree)
class BIT
{
public:
    BIT(std::vector<int> list)
    {
        // Initialize BIT with list in O(n)
        m_array = std::vector<int>(list.size() + 1, 0);
        for (int idx = 0; idx < (int)list.size(); idx++) {
            m_array[idx + 1] = list[idx];
        }

        for (int idx = 1; idx < (int)m_array.size(); idx++) {
            int idx2 = idx + (idx & -idx);
            if (idx2 < (int)m_array.size()) {
                m_array[idx2] += m_array[idx];
            }
        }
    }

    int prefix_query(int idx)
    {
        // Computes prefix sum of up to including the idx-th element
        int result = 0;
        for (++idx; idx > 0; idx -= idx & -idx) {
            result += m_array[idx];
        }
        return result;
    }

    int range_query(int from_idx, int to_idx)
    {
        // Computes the range sum between two indices (both inclusive)
        return prefix_query(to_idx) - prefix_query(from_idx - 1);
    }

    void update(int idx, int add)
    {
        // Add a value to the idx-th element
        for (++idx; idx < (int)m_array.size(); idx += idx & -idx) {
            m_array[idx] += add;
        }
    }

private:
    std::vector<int> m_array;
};
