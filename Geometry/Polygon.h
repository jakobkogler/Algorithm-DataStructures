#include <vector>
#include "Geometry.h"

template<typename T>
double signedArea(std::vector<Point<T>> polygon) {
    auto translate = Point<T>{0, 0} - polygon[0];
    for (auto& p : polygon)
        p += translate;

    double area = 0;
    for (int i = 0; i < (int)polygon.size(); i++) {
        auto p = i ? polygon[i-1] : polygon.back();
        auto q = polygon[i];
        area += (double)p.x*q.y - (double)q.x*p.y;
    }
    return area / 2;
}

Point<double> centerOfGravity(std::vector<Point<double>> polygon) {
    auto translate = Point<double>{0, 0} - polygon[0];
    for (auto& p : polygon)
        p += translate;

    double Cx = 0;
    double Cy = 0;
    for (int i = 0; i < (int)polygon.size(); i++) {
        auto p = i ? polygon[i-1] : polygon.back();
        auto q = polygon[i];
        Cx += (p.x + q.x)  * (p.x * q.y - p.y * q.x);
        Cy += (p.y + q.y)  * (p.x * q.y - p.y * q.x);
    }
    double area = signedArea(polygon);
    Point<double> center = {Cx / 6 / area, Cy / 6 / area};
    return center - translate;
}
