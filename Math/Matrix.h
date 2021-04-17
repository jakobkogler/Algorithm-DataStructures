#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>
#include <iostream>

template <typename T>
class Vector;

template<class IterT>
class StrideIter
{
public:
  using value_type = typename std::iterator_traits<IterT>::value_type;
  using reference = typename std::iterator_traits<IterT>::reference;
  using difference_type = typename std::iterator_traits<IterT>::difference_type;
  StrideIter(IterT x, int step) : m(x), step(step) { }

  StrideIter& operator++( ) { m += step; return *this; }
  StrideIter operator++(int) { auto tmp = *this; m += step; return tmp; }
  reference operator[](const difference_type n) { return m[n * step]; }
  value_type operator[](const difference_type n) const { return m[n * step]; }
  reference operator*( ) { return *m; }

  friend bool operator==(const StrideIter& x, const StrideIter& y) { return x.m == y.m; }
  friend bool operator!=(const StrideIter& x, const StrideIter& y) { return x.m != y.m; }
private:
  IterT m;
  difference_type step;
};

template <typename T>
class Vector;

template <typename IterT>
class VecView {
public:
    using T = typename IterT::value_type;
    VecView(IterT begin_, IterT end_) : begin_(begin_), end_(end_) {}
    T& operator[](int idx) { return begin_[idx]; }
    T operator[](int idx) const { return begin_[idx]; }
    T sum() const { return std::accumulate(begin(), end(), T(0)); }
    T min() const { return *std::min_element(begin(), end()); }
    T max() const { return *std::max_element(begin(), end()); }
    IterT begin() const { return begin_; }
    IterT end() const { return end_; }
    friend std::ostream& operator<<(std::ostream& os, VecView<IterT> const& vv) {
        os << "[";
        for (auto it = vv.begin(); it != vv.end(); ++it) {
            if (it != vv.begin())
                os << ", ";
            os << *it;
        }
        return os << "]";
    }
    Vector<T> copy() const {
        Vector<T> res;
        for (const auto& x : *this)
            res.push_back(x);
        return res;
    }
private:
    IterT begin_, end_;
    int step;
};

template <typename T>
class Vector : public std::vector<T> {
public:
    Vector(int n  = 0, T init = 0) : std::vector<T>(n, init) {}
    Vector(std::initializer_list<T> const& data) : std::vector<T>(data) {}
    using Vec = Vector<T>;

    T sum() const {
        return std::accumulate(this->begin(), this->end(), T(0));
    }
    T min() const {
        return *std::min_element(this->begin(), this->end());
    }
    T max() const {
        return *std::min_element(this->begin(), this->end());
    }

    friend Vec& operator+=(Vec& v, Vec const& w) {
        int n = v.size();
        for (int i = 0; i < n; i++)
            v[i] += w[i];
        return v;
    }
    friend Vec operator+(Vec const& v, Vec const& w) {
        Vec r = v;
        return r += w;
    }
    friend Vec& operator-=(Vec& v, Vec const& w) {
        int n = v.size();
        for (int i = 0; i < n; i++)
            v[i] -= w[i];
        return v;
    }
    friend Vec operator-(Vec const& v, Vec const& w) {
        Vec r = v;
        return r -= w;
    }
};

template <typename T>
class Matrix {
public:
    Matrix(int rows, int columns, T init = 0)
        : rows(rows), columns(columns), data(rows * columns, init) { }
    using Mat = Matrix<T>;
    using Vec = Vector<T>;

    Matrix(std::initializer_list<std::initializer_list<T>> const& ilist)
        : rows(ilist.size()), columns(ilist.begin()->size()), data(0) {
        data.reserve(rows * columns);
        for (auto& row : ilist) {
            for (auto& elem : row) {
                data.push_back(elem);
            }
        }
    }

    friend Mat& operator+=(Mat& A, Mat const& B) {
        A.data += B.data;
        return A;
    }
    friend Mat operator+(Mat const& A, Mat const& B) {
        Mat C = A;
        return C += B;
    }
    friend Mat& operator-=(Mat& A, Mat const& B) {
        A.data -= B.data;
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
        Mat dst(columns, rows);
        for (int i = 0; i < rows; i += blocksize) {
            for (int j = 0; j < columns; j += blocksize) {
                int iend = std::min(rows, i + blocksize);
                int jend = std::min(columns, j + blocksize);
                for (int k = i; k < iend; ++k) {
                    for (int l = j; l < jend; ++l)
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

    static Mat identity(int n) {
        Mat identity(n, n);
        for (int i = 0; i < n; i++) {
            identity.get(i, i) = 1;
        }
        return identity;
    }

    int get_idx(int row, int column) const {
        return row * columns + column;
    }
    T& get(int row, int column) {
        return data[get_idx(row, column)];
    }
    T get(int row, int column) const {
        return data[get_idx(row, column)];
    }

    using IterForView = StrideIter<typename std::vector<T>::iterator>;
    using IterForViewConst = StrideIter<typename std::vector<T>::const_iterator>;
    VecView<IterForView> row(int row_idx) {
        auto beg = IterForView(data.begin() + row_idx * columns, 1);
        auto end = IterForView(data.begin() + (row_idx + 1) * columns, 1);
        return {beg, end};
    }
    VecView<IterForViewConst> const row(int row_idx) const {
        auto beg = IterForViewConst(data.cbegin() + row_idx * columns, 1);
        auto end = IterForViewConst(data.cbegin() + (row_idx + 1) * columns, 1);
        return {beg, end};
    }
    VecView<IterForView> column(int col_idx) {
        auto beg = IterForView(data.begin() + col_idx, columns);
        auto end = IterForView(data.begin() + col_idx + rows * columns, columns);
        return {beg, end};
    }
    VecView<IterForViewConst> const column(int col_idx) const {
        auto beg = IterForViewConst(data.cbegin() + col_idx, columns);
        auto end = IterForViewConst(data.cbegin() + col_idx + rows * columns, columns);
        return {beg, end};
    }
    VecView<IterForView> operator[](int row_idx) {
        return row(row_idx);
    }
    VecView<IterForViewConst> const operator[](int row_idx) const {
        return row(row_idx);
    }
    T trace() const {
        assert(rows == columns);
        T sum = 0;
        for (int i = 0; i < rows; i++) {
            sum += get(i, i);
        }
        return sum;
    }
    T sum() const {
        return data.sum();
    }
    T min() const {
        return data.min();
    }
    T max() const {
        return data.max();
    }

    /**
     * Finds the shortest path between two adj matrices with weight infos
     */
    Mat min_weight_paths(Mat const& other, T const INF) {
        assert(columns == other.rows);

        Mat result(rows, other.columns);
        Vector<T> other_colj(other.rows);
        for (int column = 0; column < other.columns; column++) {
            for (int k = 0; k < other.rows; k++)
                other_colj[k] = other.get(k, column);

            for (int row = 0; row < rows; row++) {
                T best = INF;
                for (int k = 0; k < other.rows; k++) {
                    T left = get(row, k);
                    T right = other_colj[k];
                    if (left != INF && right != INF)
                        best = std::min(best, left + right);
                }
                result.get(row, column) = best;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, Mat const& A) {
        os << "[\n";
        for (int row = 0; row < A.rows; row++) {
            os << " [";
            for (int col = 0; col < A.columns; col++) {
                os << '\t' << A[row][col];
            }
            os << " ]";
            if (row + 1 < A.rows)
                os << ",";
            os << '\n';
        }
        os << "]";
        return os;
    }

    int rows, columns;
    Vector<T> data;
};

template <typename T>
Matrix<T> power(Matrix<T> base, long long e) {
    auto result = Matrix<T>::identity(base.rows);
    while (e) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}

/**
 * Finds the shortest paths of length k given an adjacency matrix with weight infos
 */
template <typename T>
Matrix<T> min_weight_paths(Matrix<T> base, long long k, T const INF) {
    Matrix<T> result(base.rows, base.rows);
    while (k) {
        if (k & 1)
            result = result.min_weight_paths(base, INF);
        base = base.min_weight_paths(base, INF);
        k >>= 1;
    }
    return result;
}
