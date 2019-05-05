#include <vector>

template <typename T>
class Vector : public std::vector<T> {
public:
    Vector(int n, T init = 0) : std::vector<T>(n, init) {
    }
    using Vec = Vector<T>;

    friend Vec& operator+=(Vec& v, Vec const& w) {
        int n = v.size();
        for (int i = 0; i < n; i++)
            v[i] += w[i];
        return v;
    }
    friend Vec operator+(Vec const& v, Vec const& w) {
        Vec r = v;
        return r += v;
    }
    friend Vec& operator-=(Vec& v, Vec const& w) {
        int n = v.size();
        for (int i = 0; i < n; i++)
            v[i] -= w[i];
        return v;
    }
    friend Vec operator-(Vec const& v, Vec const& w) {
        Vec r = v;
        return r -= v;
    }
};

template <typename T>
class Matrix : public std::vector<T> {
public:
    Matrix(int rows, int columns, T init = 0)
        : std::vector<T>(rows * columns, init), rows(rows), columns(columns) { }
    using Mat = Matrix<T>;
    using Vec = Vector<T>;

    friend Mat& operator+=(Mat& A, Mat const& B) {
        int size = A.rows * A.columns;
        for (int i = 0; i < size; i++)
            A[i] += B[i];
        return A;
    }
    friend Mat operator+(Mat const& A, Mat const& B) {
        Mat C = A;
        return C += B;
    }
    friend Mat& operator-=(Mat& A, Mat const& B) {
        int size = A.rows * A.columns;
        for (int i = 0; i < size; i++)
            A[i] -= B[i];
        return A;
    }
    friend Mat operator-(Mat const& A, Mat const& B) {
        Mat C = A;
        return C -= B;
    }
    friend Vec operator*(Mat const& M, Vec const& v) {
        Vec res(v.size(), 0);
        for (int i = 0; i < (int)res.size(); i++) {
            for (int j = 0; j < (int)res.size(); j++)
                res[i] += M.get(i, j) * v[j];
        }
        return res;
    }
    friend Vec operator*(Vec const& v, Mat const& M) {
        return M.transpose() * v;
    }
    Mat transpose() const {
        const int blocksize = 32;
        assert(rows % blocksize == 0);
        assert(columns % blocksize == 0);

        Mat dst(columns, rows);
        for (int i = 0; i < rows; i += blocksize) {
            for (int j = 0; j < columns; j += blocksize) {
                for (int k = i; k < i + blocksize; ++k) {
                    for (int l = j; l < j + blocksize; ++l)
                        dst.get(l, k) = get(k, l);
                }
            }
        }
        return dst;
    }
    friend Mat operator*(Mat const& A, Mat const& B) {
        assert(A.columns == B.rows);

        Matrix<T> C(A.rows, B.columns);

        std::vector<T> Bcolj(B.rows);
        for (int j = 0; j < B.columns; j++) {
            for (int k = 0; k < B.rows; k++)
                Bcolj[k] = B.get(k, j);

            for (int i = 0; i < A.rows; i++) {
                T s = 0;
                for (int k = 0; k < B.rows; k++)
                    s += A.get(i, k) * Bcolj[k];
                C.get(i, j) = s;
            }
        }
        return C;
    }
    Mat get_quarter(int i, int j) const {
        int offset_x = i == 2 ? rows / 2 : 0;
        int offset_y = j == 2 ? columns / 2 : 0;
        Mat res(rows / 2, columns / 2);
        for (int x = 0; x < res.rows; x++)
            for (int y = 0; y < res.columns; y++)
                res.get(x, y) = get(x + offset_x, y + offset_y);
        return res;
    }
    void set_quarter(Mat const& A, int i, int j) {
        int offset_x = i == 2 ? rows / 2 : 0;
        int offset_y = j == 2 ? columns / 2 : 0;
        for (int x = 0; x < A.rows; x++)
            for (int y = 0; y < A.columns; y++)
                get(x + offset_x, y + offset_y) = A.get(x, y);
    }
    friend Mat strassen(Mat const& A, Mat const& B) {
        assert(A.rows == A.columns && A.columns == B.rows && B.rows == B.columns && (A.rows & (A.rows-1)) == 0);
        
        if (A.rows <= 64)
            return A * B;

        Mat C(A.rows, B.columns);
        auto A11 = A.get_quarter(1, 1);
        auto A12 = A.get_quarter(1, 2);
        auto A21 = A.get_quarter(2, 1);
        auto A22 = A.get_quarter(2, 2);
        auto B11 = B.get_quarter(1, 1);
        auto B12 = B.get_quarter(1, 2);
        auto B21 = B.get_quarter(2, 1);
        auto B22 = B.get_quarter(2, 2);
        auto M1 = strassen(A11 + A22, B11 + B22);
        auto M2 = strassen(A21 + A22, B11);
        auto M3 = strassen(A11, B12 - B22);
        auto M4 = strassen(A22, B21 - B11);
        auto M5 = strassen(A11 + A12, B22);
        auto M6 = strassen(A21 - A11, B11 + B12);
        auto M7 = strassen(A12 - A22, B21 + B22);
        auto C11 = M1 + M4 - M5 + M7;
        auto C12 = M3 + M5;
        auto C21 = M2 + M4;
        auto C22 = M1 - M2 + M3 + M6;
        C.set_quarter(C11, 1, 1);
        C.set_quarter(C12, 1, 2);
        C.set_quarter(C21, 2, 1);
        C.set_quarter(C22, 2, 2);
        return C;
    }
    int get_idx(int row, int column) const {
        return row * columns + column;
    }
    T& get(int row, int column) {
        return (*this)[get_idx(row, column)];
    }
    T get(int row, int column) const {
        return (*this)[get_idx(row, column)];
    }
    int rows, columns;
};
