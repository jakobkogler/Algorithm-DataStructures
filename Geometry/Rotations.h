#include "Geometry.h"
#include "../Math/Matrix.h"

template <typename T>
Matrix<T> createTranslation2D(Point<T> from, Point<T> to) {
    auto vec = to - from;
    Matrix<T> translation(3, 3);
    for (int i = 0; i < 2; i++) {
        translation[i][i] = 1;
        translation[i][2] = vec[i];
    }
    translation[2][2] = 1;
    return translation;
}

Matrix<double> createRotation2D(double angleRadian) {
    Matrix<double> rotation(3, 3);
    rotation[0][0] = rotation[1][1] = std::cos(angleRadian);
    rotation[0][1] = -std::sin(angleRadian);
    rotation[1][0] = -rotation[0][1];
    rotation[2][2] = 1;
    return rotation;
}

template<typename T>
Point<T> operator*(Matrix<T> const& M, Point<T> p) {
    std::vector<T> v = {p.x, p.y, 1};
    auto res = M * v;
    return {res[0], res[1]};
}
