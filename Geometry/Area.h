#include "Geometry.h"

double det(Vector const& v, Vector const& u) {
    return v.x*u.y - v.y*u.x;
}

double area_parallelogram(Point const& p1, Point const& p2, Point const& p3) {
    return std::abs(det(p2 - p1, p3 - p2));
}

double area_triangle(Point const& p1, Point const& p2, Point const& p3) {
    return area_parallelogram(p1, p2, p3) / 2;
}
