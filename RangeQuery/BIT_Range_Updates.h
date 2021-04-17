#include <vector>

template <typename T = long long>
class BIT_Range_Updates {
public:
    BIT_Range_Updates(int n) : n(n + 1) {
        m_array.resize(n + 1, 0);
    }

    BIT_Range_Updates(const std::vector<T>& list) : n(list.size() + 1) {
        n = list.size() + 1;
        m_array = std::vector<T>(n, 0);
        for (int i = 0; i < n; i++)
            m_array[i+1] = list[i] - (i > 0 ? list[i-1] : 0);
        
        for (int idx = 1; idx < n; idx++) {
            int idx2 = idx + (idx & -idx);
            if (idx2 < (int)m_array.size()) {
                m_array[idx2] += m_array[idx];
            }
        }
    }

    T value(int idx) {
        T result = 0;
        for (++idx; idx > 0; idx -= idx & -idx)
            result += m_array[idx];
        return result;
    }

    // add x to the range [l, r)
    void range_add(int l, int r, T x) {
        suffix_add(l, x);
        suffix_add(r, -x);
    }

    // add x to the suffix starting at index idx
    void suffix_add(int idx, T add) {
        for (++idx; idx < n; idx += idx & -idx)
            m_array[idx] += add;
    }

private:
    std::vector<T> m_array;
    int n;
};
