#include <vector>

class SparseTable
{
public:
    SparseTable(const std::vector<int> &v)
    {
        log_table.assign(v.size() + 1, 0);
        for (auto i = 2UL; i < log_table.size(); i++)
            log_table[i] = log_table[i / 2] + 1;

        sparse_table.assign(log_table.back() + 1, std::vector<int>(v.size()));
        sparse_table[0] = v;
        for (auto row = 1UL; row < sparse_table.size(); row++) {
            for (auto i = 0UL; i + (1 << row) <= v.size(); i++) {
                sparse_table[row][i] =
                    std::min(sparse_table[row - 1][i],
                             sparse_table[row - 1][i + (1 << (row - 1))]);
            }
        }
    }

    int minimum(int l, int r)
    {
        int log = log_table[r - l];
        return std::min(sparse_table[log][l],
                        sparse_table[log][r - (1 << log) + 1]);
    }

private:
    std::vector<int> log_table;
    std::vector<std::vector<int>> sparse_table;
};
