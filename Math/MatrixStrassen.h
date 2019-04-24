#include "Matrix.h"

template <typename T>
Matrix<T> operator-(Matrix<T> const& A, Matrix<T> const& B) {
    auto res = A;
    for (int i = 0; i < (int)res.size(); i++) {
        for (int j = 0; j < (int)res[0].size(); j++) {
            res[i][j] -= B[i][j];
        }
    }
    return res;
}

template <typename T>
Matrix<T> get_quarter(Matrix<T> const& M, int quadrant) {
    int n = M.size() / 2;
    int offset_x = 0;
    int offset_y = 0;
    if (quadrant > 2)
        offset_x = n;
    if ((quadrant & 2) == 0)
        offset_y = n;

    Matrix<T> res(n, n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            res[x][y] = M[x + offset_x][y + offset_y];
        }
    }
    return res;
}

template <typename T>
void set_quarter(Matrix<T> & M, Matrix<T> const& A, int quadrant) {
    int n = M.size() / 2;
    int offset_x = 0;
    int offset_y = 0;
    if (quadrant > 2)
        offset_x = n;
    if ((quadrant & 2) == 0)
        offset_y = n;

    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            M[x + offset_x][y + offset_y] = A[x][y];
        }
    }
}

template <typename T>
Matrix<T> strassen(Matrix<T> const& A, Matrix<T> const& B, int max_trivial=64) {
    int n = A.size();
    if (n <= max_trivial)
        return A * B;

    Matrix<T> C(n, n);
    auto A11 = get_quarter(A, 2);
    auto A12 = get_quarter(A, 1);
    auto A21 = get_quarter(A, 3);
    auto A22 = get_quarter(A, 4);
    auto B11 = get_quarter(B, 2);
    auto B12 = get_quarter(B, 1);
    auto B21 = get_quarter(B, 3);
    auto B22 = get_quarter(B, 4);
    auto M1 = strassen(A11 + A22, B11 + B22, max_trivial);
    auto M2 = strassen(A21 + A22, B11, max_trivial);
    auto M3 = strassen(A11, B12 - B22, max_trivial);
    auto M4 = strassen(A22, B21 - B11, max_trivial);
    auto M5 = strassen(A11 + A12, B22, max_trivial);
    auto M6 = strassen(A21 - A11, B11 + B12, max_trivial);
    auto M7 = strassen(A12 - A22, B21 + B22, max_trivial);
    auto C11 = M1 + M4 - M5 + M7;
    auto C12 = M3 + M5;
    auto C21 = M2 + M4;
    auto C22 = M1 - M2 + M3 + M6;
    set_quarter(C, C11, 2);
    set_quarter(C, C12, 1);
    set_quarter(C, C21, 3);
    set_quarter(C, C22, 4);
    return C;
}
