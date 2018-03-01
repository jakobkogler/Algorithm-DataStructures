#include <vector>
#include <limits>

template <typename T>
class Matrix : public std::vector<std::vector<T>> {
public:
    Matrix(int n, int m, T init) 
        : std::vector<std::vector<T>>(n, std::vector<T>(m, init)) {}
};

constexpr auto INF = std::numeric_limits<long long>::max();

template <typename T>
std::vector<T> operator*(std::vector<T> const& v, Matrix<T> const& M) {
    auto res = v;
    for (int i = 0; i < (int)res.size(); i++) {
        T cost = INF;
        for (int j = 0; j < (int)res.size(); j++)
            cost = min(cost, v[j] + M[j][i]);
        res[i] = cost;
    }
    return res;
}

template <typename T>
Matrix<T> operator*(Matrix<T> const& A, Matrix<T> const& B) {
    auto res = A;
    for (int i = 0; i < (int)res.size(); i++)
        res[i] = A[i] * B;
    return res;
}
