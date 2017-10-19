#include <algorithm>
#include <vector>

int square(int value)
{
    return value * value;
}

class Point
{
public:
    Point(int x, int y) : x(x), y(y) {}

    int dist2(Point const &other) const
    {
        return square(x - other.x) + square(y - other.y);
    }

    // ccw > 0 => counter-clockwise turn
    // ccw = 0 => colinear
    // ccw < 0 => clockwise turn
    static int ccw(Point const &p1, Point const &p2, Point const &p3)
    {
        return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    }

    int x, y;
};

std::vector<Point> GrahamScan(std::vector<Point> points)
{
    sort(points.begin(), points.end(), [](auto const &p, auto const &q) {
        return std::make_pair(p.y, p.x) < std::make_pair(q.y, q.x);
    });
    sort(points.begin() + 1, points.end(), [ref = points[0]](auto const &p, auto const &q) {
         int orientation = Point::ccw(ref, p, q);
         if (orientation < 0)
             return true;
         if (orientation == 0)
             return ref.dist2(p) < ref.dist2(q);
         return false;
     });

    std::vector<Point> result;
    result.push_back(points[0]);
    result.push_back(points[1]);
    int sz = 2;
    for (int i = 2; i < static_cast<int>(points.size()); i++) {
        while (sz > 1 && Point::ccw(result[sz - 2], result[sz - 1], points[i]) >= 0) {
            sz--;
            result.pop_back();
        }
        sz++;
        result.push_back(points[i]);
    }

    return result;
}
