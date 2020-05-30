#include "SlowPolynomial.h"
#include "NevilleInterpolationPolynomial.h"

template<typename T>
Polynomial<T> compute_power_sum_polynomial(int exp) {
    std::vector<Point<T>> points;
    int req = exp + 2;
    T sum = 0;
    for (int j = 0; j < req; j++) {
        sum += power(T(j), exp);
        points.push_back({j, sum});
    }
    return Nevilles(points);
}
