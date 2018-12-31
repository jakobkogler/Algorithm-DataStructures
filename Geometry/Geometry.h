#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>

constexpr double EPS = 1e-9;

template <class T>
T det(T a11, T a12, T a21, T a22) {
    return a11 * a22 - a12 * a21;
}

template <class T>
T sq(T x) {
    return x * x;
}

template <class T>
class Vector {
public:
    Vector(T x=0, T y=0) : x(x), y(y) {}

    Vector& operator+=(Vector const& v) { x += v.x; y += v.y; return *this; }
    Vector operator+(Vector const& v) const { Vector u = *this; u += v; return u; }
    Vector& operator-=(Vector const& v) { x -= v.x; y -= v.y; return *this; }
    Vector operator-(Vector const& v) const { Vector u = *this; u -= v; return u; }
    Vector& operator*=(T const c) { x *= c; y *= c; return *this; }
    Vector operator*(T const& c) const { Vector u = *this; u *= c; return u; }
    Vector& operator/=(T const c) { x /= c; y /= c; return *this; }
    Vector operator/(T const& c) const { Vector u = *this; u /= c; return u; }
    bool operator==(Vector const& v) const { return std::abs(x - v.x) < EPS && std::abs(y - v.y) < EPS; }
    bool operator!=(Vector const& v) const { return !(*this == v); }
    bool operator<(Vector const& v) const { return x < v.x || (x == v.x && y < v.y); }
    T operator*(Vector const& v) const { return x*v.x + y*v.y; }
    T operator[](int idx) const { return idx ? y : x; }

    T length2() const {
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
        double angle = v.atan2_angle() - atan2_angle();
        const double PI = acos(-1);
        if (angle > PI)
            angle -= PI;
        if (angle <= -PI)
            angle += PI;
        return angle;
    }

    T x, y;
};

template <>
bool Vector<long long>::operator==(Vector<long long> const& v) const {
    return x == v.x && y == v.y; 
}

template <>
void Vector<long long>::normalize() {
    long long g = std::gcd(x, y);
    x /= g;
    y /= g;
}

template <class T>
class Point {
public:
    Point(T x=0, T y=0) : x(x), y(y) {}

    Point& operator+=(Vector<T> const& v) { x += v.x; y += v.y; return *this; }
    Point operator+(Vector<T> const& v) const { Point p = *this; p += v; return p; }
    Point& operator-=(Vector<T> const& v) { x -= v.x; y -= v.y; return *this; }
    Point operator-(Vector<T> const& v) const { Point p = *this; p -= v; return p; }
    Vector<T> operator-(Point const& p) const { return {x - p.x, y - p.y}; }
    bool operator==(Point const& p) { return std::abs(x - p.x) < EPS && std::abs(y - p.y) < EPS; }
    bool operator!=(Point const& p) const { return !(*this == p); }
    bool operator<(Point const& p) const { return x < p.x || (x == p.x && y < p.y); }
    friend std::istream& operator>>(std::istream& is, Point<T>& pt) { return is >> pt.x >> pt.y; }
    friend std::ostream& operator<<(std::ostream& os, Point<T> const& pt) { return os << pt.x << " " << pt.y; }

    T x, y;
};

template <>
bool Point<long long>::operator==(Point const& p) {
    return x == p.x && y == p.y;
}

template <class T>
class Line {
public:
    // ax + by + c = 0
    Line(T a, T b, T c) : a(a), b(b), c(c) {}
    // y = kx + d
    Line(T k, T d) : a(k), b(-1), c(d) {}
    // point, point
    Line(Point<T> p, Point<T> q) 
        : a(q.y - p.y), 
          b(p.x - q.x),
          c(p.y * q.x - p.x * q.y) {}
    // point + vector
    Line(Point<T> p, Vector<T> v) :
        a(-v.y), 
        b(v.x), 
        c(p.x*v.y - p.y*v.x) {}

    bool parallel(Line const& other) const {
        return std::abs(det(a, b, other.a, other.b)) < EPS;
    }

    Point<T> intersect(Line const& other) const {
        double d = det(a, b, other.a, other.b);
        double x = -det(c, b, other.c, other.b) / d;
        double y = -det(a, c, other.a, other.c) / d;
        return {x, y};
    }

    double distance(Point<T> const& p) const {
        return std::abs(a*p.x + b*p.y + c) / Vector<T>(a, b).length();
    }

    virtual bool contains(Point<T> const& p) const {
        return std::abs(a*p.x + b*p.y + c) < EPS;
    }

    T a, b, c;
};

template <>
bool Line<long long>::parallel(Line<long long> const& other) const {
    return det(a, b, other.a, other.b) == 0;
}

template <>
bool Line<long long>::contains(Point<long long> const& p) const {
    return a*p.x + b*p.y + c == 0;
}

template <class T>
class Segment : public Line<T> {
public:
    Segment(Point<T> p, Point<T> q) : Line<T>(p, q), p(p), q(q) {}

    bool between_1d(T const x, T const b1, T const b2) const {
        return min(b1, b2) <= x && x <= max(b1, b2);
    }

    bool contains(Point<T> const& pt) const override {
        if (!between_1d(pt.x, p.x, q.x) || !between_1d(pt.y, p.y, q.y))
            return false;
        return Line<T>::contains(pt);
    }

    Point<T> p, q;
};

template <class T>
class Circle {
public:
    Circle(Point<T> m, T r) : m(m), r(r) {}
    Circle(T r) : m({0, 0}), r(r) {}

    bool inside(Point<T> p) {
        return (p - m).length2() < r*r;
    }

    std::vector<Point<T>> intersect(Line<T> line) {
        std::vector<Point<T>> intersections;
        T a = line.a;
        T b = line.b;
        T c = a*m.x + b*m.y + line.c;
        double d2 = sq(a) + sq(b);
        Point<T> closest(-a*c/d2, -b*c/d2);

        T diff = sq(r)*d2 - sq(c);
        if (diff > EPS) {
            double d = sq(r) - sq(c)/d2;
            double factor = sqrt(d / d2);
            intersections.push_back(closest + Vector<T>{b, -a} * factor);
            intersections.push_back(closest + Vector<T>{-b, a} * factor);
        } else if (std::abs(diff) <= EPS) {
            intersections.push_back(closest);
        }

        for (auto& p : intersections)
            p += Vector<T>{m.x, m.y};
        return intersections;
    }

    std::vector<Point<T>> intersect(Circle other) {
        Line<T> line(2*(other.m.x - m.x), 
                  2*(other.m.y - m.y), 
                  sq(other.r) - sq(r) + sq(m.x) + sq(m.y)
                  - sq(other.m.x) - sq(other.m.y));
        return intersect(line);
    }

    Point<T> m;
    T r;
};
