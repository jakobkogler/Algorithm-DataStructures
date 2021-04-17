#include <vector>

template<typename T = long long>
class BIT {
// Implementation of a Binary Indexed Tree (Fennwick Tree)
public:
    BIT(int n) : n(n + 1) {
        m_array = std::vector<T>(this->n, 0);
    }

    BIT(const std::vector<T>& list) {
        // Initialize BIT with list in O(n)
        m_array = std::vector<T>(n, 0);
        std::copy(list.begin(), list.end(), m_array.begin() + 1);
        
        for (int idx = 1; idx < n; idx++) {
            int idx2 = idx + (idx & -idx);
            if (idx2 < n) {
                m_array[idx2] += m_array[idx];
            }
        }
    }

    // Computes sum of the range [0, idx)
    int prefix_query(int idx) {
        int result = 0;
        for (; idx > 0; idx -= idx & -idx) {
            result += m_array[idx];
        }
        return result;
    }

    // Computes the range sum of the range [l, r)
    int range_query(int l, int r) {
        return prefix_query(r) - prefix_query(l);
    }

    // Add a value to the idx-th element
    void update(int idx, int add) {
        for (++idx; idx < n; idx += idx & -idx) {
            m_array[idx] += add;
        }
    }

private:
    std::vector<T> m_array;
    int n;
};
