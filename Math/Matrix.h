#include <vector>

template <typename T>
class Matrix : public std::vector<std::vector<T>> {
public:
    Matrix(int n, int m, T init=0) 
        : std::vector<std::vector<T>>(n, std::vector<T>(m, init)) {}
};

template <typename T>
std::vector<T> operator*(std::vector<T> const& v, Matrix<T> const& M) {
    std::vector<T> res(v.size(), 0);
    for (int i = 0; i < (int)res.size(); i++) {
        for (int j = 0; j < (int)res.size(); j++)
            res[i] += v[j] * M[j][i];
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

template <typename T>
Matrix<T> operator+(Matrix<T> const& A, Matrix<T> const& B) {
    auto res = A;
    for (int i = 0; i < (int)res.size(); i++) {
        for (int j = 0; j < (int)res[0].size(); j++) {
            res[i][j] += B[i][j];
        }
    }
    return res;
}
