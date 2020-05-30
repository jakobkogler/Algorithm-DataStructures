#include "SlowPolynomial.h"

template<typename T>
struct Point {
    T x, y;
};

template<typename T>
Polynomial<T> Nevilles(std::vector<Point<T>> const& points) {
    // interpolate a polynomial of degree points.size()-1
    int degree = points.size() - 1;
    Polynomial<T> x({0, 1});
    std::vector<Polynomial<T>> p(degree + 1);
    for (int i = 0; i <= degree; i++)
        p[i] = Polynomial<T>({points[i].y});
    for (int d = 1; d <= degree; d++) {
        for (int i = 0; i <= degree - d; i++) {
            int j = i + d;
            p[i] = (Polynomial<T>({-points[j].x, 1}) * p[i] - Polynomial<T>({-points[i].x, 1}) * p[i + 1]) / (points[i].x - points[j].x);
        }
    }
    return p[0];
}
