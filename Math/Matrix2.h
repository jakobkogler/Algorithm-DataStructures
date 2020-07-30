#include <array>

template <typename T>
class Matrix2 {
public:
    using Arr4 = std::array<T, 4>;

    Matrix2(Arr4 const& data) : data(data) {}

    Matrix2& operator+=(Matrix2 const& other) {
        for (int i = 0; i < 4; i++)
            data[i] += other.data[i];
        return *this;
    }

    Matrix2 operator+(Matrix2 const& other) {
        Matrix2 cpy = *this;
        return cpy += other;
    }

    Matrix2& operator-=(Matrix2 const& other) {
        for (int i = 0; i < 4; i++)
            data[i] -= other.data[i];
        return *this;
    }

    Matrix2 operator-(Matrix2 const& other) {
        Matrix2 cpy = *this;
        return cpy -= other;
    }

    Matrix2 operator*(Matrix2 const& other) {
        return Matrix2({data[0]*other.data[0] + data[1]*other.data[2],
                    data[0]*other.data[1] + data[1]*other.data[3],
                    data[2]*other.data[0] + data[3]*other.data[2],
                    data[2]*other.data[1] + data[3]*other.data[3]});
    }

    Arr4 data;
};

template <typename T>
Matrix2<T> power(Matrix2<T> base, long long e) {
    auto result = Matrix2<T>({1, 0, 0, 1});
    while (e) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}
