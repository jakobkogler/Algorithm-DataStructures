#include <vector>
#include <ostream>
#include "FFT.h"

template<typename T>
class Polynomial {
public:
    Polynomial() : coeffs({}) {}

    Polynomial(std::vector<T> coeffs) : coeffs(coeffs) {}

    Polynomial<T>& operator+=(Polynomial<T> const& other) {
        coeffs.resize(max(coeffs.size(), other.coeffs.size()));
        for (int i = 0; i < coeffs.size(); i++) {
            coeffs[i] += other.coeffs[i];
        }
        shorten();
        return *this;
    }

    Polynomial<T> operator+(Polynomial<T> const& other) const {
        return Polynomial<T>(*this) += other;
    }

    Polynomial<T>& operator-=(Polynomial<T> const& other) {
        coeffs.resize(max(coeffs.size(), other.coeffs.size()));
        for (int i = 0; i < coeffs.size(); i++) {
            coeffs[i] -= other.coeffs[i];
        }
        shorten();
        return *this;
    }

    Polynomial<T> operator-(Polynomial<T> const& other) const {
        return Polynomial<T>(*this) -= other;
    }

    Polynomial<T>& operator*=(T const& x) {
        for (T& coeff : coeffs)
            coeff *= x;
        shorten();
        return *this;
    }

    Polynomial<T> operator*(T const& x) const {
        return Polynomial<T>(*this) *= x;
    }

    Polynomial<T>& operator*=(Polynomial<T> const& other) {
        int result_deg = deg() + other.deg() - 1;
        if (result_deg <= 200) {
            coeffs = multiply_brute_force(coeffs, other.coeffs, result_deg);
        } else {
            coeffs = fft_multiply(coeffs, other.coeffs);
        }
        shorten();
        return *this;
    }

    Polynomial<T> operator*(Polynomial<T> const& other) const {
        return Polynomial<T>(*this) *= other;
    }

    Polynomial<T>& operator/=(T const& x) {
        return *this *= (1 / x);
    }

    Polynomial<T> operator/(T const& x) const {
        return Polynomial<T>(*this) /= x;
    }

    Polynomial<T> operator/(Polynomial<T> const& other) const {
        return divide(other);
    }

    Polynomial<T>& operator/=(Polynomial<T> const& other) {
        auto res = divide(other);
        coeffs.swap(res.coeffs);
        return *this;
    }

    Polynomial<T> operator%(Polynomial<T> const& other) const {
        return divide_modulo(other).second;
    }

    Polynomial<T>& operator%=(Polynomial<T> const& other) {
        auto res = divide_modulo(other).second;
        coeffs.swap(res.coeffs);
        return *this;
    }

    T evaluate(T const& x) const {  // Horner's method
        T res = 0;
        for (int i = coeffs.size() - 1; i >= 0; i--)
            res = res * x + coeffs[i];
        return res;
    }

    T operator()(T const& x) const {
        return evaluate(x);
    }

    Polynomial<T> derivation() const {
        auto cpy = coeffs;
        for (int i = 1; i < cpy.size(); i++) {
            cpy[i-1] = cpy[i] * i;
        }
        cpy.pop_back();
        return Polynomial<T>(cpy);
    }

    /**
     * Computes the poloynomial modulo x^n (so just the first n coefficitiens)
     */
    Polynomial<T> operator%(int n) const {
        if (n == 0)
            return Polynomial<T>({0});
        if (coeffs.size() <= n)
            return *this;
        Polynomial<T> ret({coeffs.begin(), coeffs.begin() + n});
        ret.shorten();
        return ret;
    }

    /**
     * Computes the reciprocal polynomial modulo x^n
     */
    Polynomial<T> reciprocal(int n) const {
        assert(coeffs[0] != 0);
        int sz = 1;
        Polynomial<T> R({T(1) / coeffs[0]});
        while (sz < n) {
            sz *= 2;
            R = R * 2 - R * R * (*this % sz);
        }
        return R % n;
    }

    /**
     * reverse the coefficients of the polynomial
     */
    Polynomial<T> rev() const {
        Polynomial<T> ret({coeffs.rbegin(), coeffs.rend()});
        ret.shorten();
        return ret;
    }

    /**
     * divide this polynomial by g
     */
    Polynomial<T> divide(Polynomial<T> const& g) const {
        /* if ( */
        int n = deg() - g.deg() + 1;
        return (rev() * g.rev().reciprocal(n) % n).rev();
    }

    /**
     * Computes the divisor and remainder of a division by g
     */
    std::pair<Polynomial<T>, Polynomial<T>> divide_modulo(Polynomial<T> const& g) const {
        /* if ( */
        auto q = divide(g);
        auto r = *this - g * q;
        return std::make_pair(q, r);
    }

    int deg() const {
        if (coeffs.size() == 1)
            return coeffs[0] != 0 ? 1 : 0;
        return coeffs.size();
    }

    friend std::ostream& operator<<(std::ostream& os, Polynomial<T> const& p) {
        for (int i = p.coeffs.size() - 1; i >= 0; i--) {
            os << p.coeffs[i];
            if (i)
                os << "*x";
            if (i > 1)
                os << "^" << i;
            if (i)
                os << " + ";
        }
        return os;
    }

    void shorten() {
        while (coeffs.back() == 0 && coeffs.size() > 1)
            coeffs.pop_back();
    }

    std::vector<T> coeffs;
    // order of coefficients is small to large

private:
    std::vector<T> multiply_brute_force(std::vector<T> const& a, std::vector<T> const& b, int sz) {
        std::vector<T> result(sz);
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)b.size(); j++) {
                result[i + j] += (T)a[i] * b[j];
            }
        }
        return result;
    }
};
