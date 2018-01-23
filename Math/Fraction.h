#include <algorithm>

class Fraction
{
public:
    Fraction() : a(0), b(1) {}
    Fraction(long long a) : a(a), b(1) {}
    Fraction(long long num, long long den) : a(num), b(den) {
        long long g = gcd(a, b);
        a /= g;
        b /= g;
    }
    
    Fraction& operator+=(Fraction const& other) {
        long long g = gcd(b, other.b);
        a = a * (other.b / g) + (b / g) * other.a;
        b = b / g * other.b;
        return *this;
    }

    Fraction operator+(Fraction const& other) const {
        Fraction t = *this; t += other; return t;
    }

    Fraction& operator-=(Fraction const& other) {
        long long g = gcd(b, other.b);
        a = a * (other.b / g) - (b / g) * other.a;
        b = b / g * other.b;
        return *this;
    }

    Fraction operator-(Fraction const& other) const {
        Fraction t = *this; t -= other; return t;
    }

    Fraction& operator*=(Fraction const& other) {
        long long g1 = gcd(a, other.b);
        long long g2 = gcd(b, other.a);
        a = (a / g1) * (other.a / g2);
        b = (b / g2) * (other.b / g1);
        return *this;
    }

    Fraction operator*(Fraction const& other) const {
        Fraction t = *this; t *= other; return t;
    }

    Fraction& operator/=(Fraction const& other) {
        *this *= Fraction(other.b, other.a);
        return *this;
    }

    Fraction operator/(Fraction const& other) const {
        Fraction t = *this; t /= other; return t;
    }

    bool operator<(Fraction const& other) const {
        return a * other.b < other.a * b;
    }

    bool operator<=(Fraction const& other) const {
        return a * other.b <= other.a * b;
    }

    bool operator>(Fraction const& other) const {
        return a * other.b > other.a * b;
    }

    bool operator>=(Fraction const& other) const {
        return a * other.b >= other.a * b;
    }

    bool operator==(Fraction const& other) const {
        return a * other.b == other.a * b;
    }

    bool operator!=(Fraction const& other) const {
        return a * other.b != other.a * b;
    }

    long long floor() const {
        return a / b;
    }

    static long long gcd(long long a, long long b) {
        while (b > 0) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }

    long long a, b;
};

Fraction operator+(long long x, Fraction const& f) { return Fraction(x) + f; }
Fraction operator-(long long x, Fraction const& f) { return Fraction(x) - f; }
Fraction operator*(long long x, Fraction const& f) { return Fraction(x) * f; }
Fraction operator/(long long x, Fraction const& f) { return Fraction(x) / f; }
bool operator<(long long x, Fraction const& f) { return Fraction(x) < f; }
bool operator<=(long long x, Fraction const& f) { return Fraction(x) <= f; }
bool operator>(long long x, Fraction const& f) { return Fraction(x) > f; }
bool operator>=(long long x, Fraction const& f) { return Fraction(x) >= f; }
bool operator==(long long x, Fraction const& f) { return Fraction(x) == f; }
bool operator!=(long long x, Fraction const& f) { return Fraction(x) != f; }
