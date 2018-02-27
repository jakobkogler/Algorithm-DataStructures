#include <cmath>
#include <vector>

static constexpr double EPS = 1e-9;

static double det(double a11, double a12, double a21, double a22) {
    return a11 * a22 - a12 * a21;
}

static double sq(double x) {
    return x * x;
}

class Vector {
public:
    Vector(double x, double y) : x(x), y(y) {}

    Vector& operator+=(Vector const& v) { x += v.x; y += v.y; return *this; }
    Vector operator+(Vector const& v) const { Vector u = *this; u += v; return u; }
    Vector& operator-=(Vector const& v) { x -= v.x; y -= v.y; return *this; }
    Vector operator-(Vector const& v) const { Vector u = *this; u -= v; return u; }
    Vector& operator*=(double const c) { x *= c; y *= c; return *this; }
    Vector operator*(double const& c) const { Vector u = *this; u *= c; return u; }
    Vector& operator/=(double const c) { x /= c; y /= c; return *this; }
    Vector operator/(double const& c) const { Vector u = *this; u /= c; return u; }
    bool operator==(Vector const& v) const { return std::abs(x - v.x) < EPS && std::abs(y - v.y) < EPS; }
    bool operator!=(Vector const& v) const { return !(*this == v); }
    double operator*(Vector const& v) const { return x*v.x + y*v.y; }

    double length2() const {
        return sq(x) + sq(y);
    }

    double length() const {
        return sqrt(length2());
    }

    double atan2_angle() const {
        return atan2(y, x);
    }

    void normalize() {
        double l = length();
        x /= l;
        y /= l;
    }

    double angle(Vector const& v) const {
        return acos(*this * v / length() / v.length());
    }

    double x, y;
};

class Point {
public:
    Point(double x, double y) : x(x), y(y) {}

    Point& operator+=(Vector const& v) { x += v.x; y += v.y; return *this; }
    Point operator+(Vector const& v) const { Point p = *this; p += v; return p; }
    Point& operator-=(Vector const& v) { x -= v.x; y -= v.y; return *this; }
    Point operator-(Vector const& v) const { Point p = *this; p -= v; return p; }
    Vector operator-(Point const& p) const { return {x - p.x, y - p.y}; }
    bool operator==(Point const& p) { return std::abs(x - p.x) < EPS && std::abs(y - p.y) < EPS; }
    bool operator!=(Point const& p) { return !(*this == p); }

    double x, y;
};

class Line {
public:
    // ax + by + c = 0
    Line(double a, double b, double c) : a(a), b(b), c(c) {}
    // y = kx + d
    Line(double k, double d) : a(k), b(-1), c(d) {}
    // point, point
    Line(Point p, Point q) {
        double x_diff = q.x - p.x;
        if (std::abs(x_diff) < EPS) {
            a = 1;
            b = 0;
            c = -q.x;
        } else {
            a = q.y - p.y;
            b = p.x - q.x;
            c = p.y * q.x - p.x * q.y;
        }
    }
    // point + vector
    Line(Point p, Vector v) {
        a = -v.y;
        b = v.x;
        c = p.x*v.y - p.y*v.x;
    }

    bool parallel(Line const& other) const {
        return std::abs(det(a, b, other.a, other.b)) < EPS;
    }

    Point intersect(Line const& other) const {
        double d = det(a, b, other.a, other.b);
        double x = -det(c, b, other.c, other.b) / d;
        double y = -det(a, c, other.a, other.c) / d;
        return {x, y};
    }

    double distance(Point const& p) const {
        return std::abs(a*p.x + b*p.y + c) / Vector(a, b).length();
    }

    double a, b, c;
};

class Circle {
public:
    Circle(Point m, double r) : m(m), r(r) {}
    Circle(double r) : m({0, 0}), r(r) {}

    bool inside(Point p) {
        return (p - m).length2() < r*r;
    }

    std::vector<Point> intersect(Line line) {
        std::vector<Point> intersections;
        double a = line.a;
        double b = line.b;
        double c = a*m.x + b*m.y + line.c;
        double d2 = sq(a) + sq(b);
        Point closest(-a*c/d2, -b*c/d2);

        double diff = sq(r)*d2 - sq(c);
        if (diff > EPS) {
            double d = sq(r) - sq(c)/d2;
            double factor = sqrt(d / d2);
            intersections.push_back(closest + Vector{b, -a} * factor);
            intersections.push_back(closest + Vector{-b, a} * factor);
        } else if (std::abs(diff) <= EPS) {
            intersections.push_back(closest);
        }

        for (auto& p : intersections)
            p += Vector{m.x, m.y};
        return intersections;
    }

    std::vector<Point> intersect(Circle other) {
        Line line(2*(other.m.x - m.x), 
                  2*(other.m.y - m.y), 
                  sq(other.r) - sq(r) + sq(m.x) + sq(m.y)
                  - sq(other.m.x) - sq(other.m.y));
        return intersect(line);
    }

    Point m;
    double r;
};
