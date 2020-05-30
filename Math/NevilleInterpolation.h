#include <vector>

template<typename T>
struct Point {
    T x, y;
};

template<typename T>
T Nevilles(std::vector<Point<T>> const& points, T x) {
    // interpolate a polynomial of degree points.size()-1, and evaluate it at x
    // let p[i][j](x) be the polynomial interpolated with the points i..j
    // then - p[i][i](x) = y_i
    //      - p[i][j](x) = ((x - x_j) * p[i][j-1](x) - (x - x_i) * p[i+1][j](x)) / (x-i - x_j)
    int degree = points.size() - 1;
    std::vector<T> p(degree + 1);
    for (int i = 0; i <= degree; i++)
        p[i] = points[i].y;
    for (int d = 1; d <= degree; d++) {
        for (int i = 0; i <= degree - d; i++) {
            int j = i + d;
            p[i] = ((x - points[j].x) * p[i] - (x - points[i].x) * p[i+1]) / (points[i].x - points[j].x);
        }
    }
    return p[0];
}
