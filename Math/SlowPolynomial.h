#include <vector>
#include <iostream>

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
    Polynomial<T> operator+(Polynomial<T> const& other) {
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
    Polynomial<T> operator-(Polynomial<T> const& other) {
        return Polynomial<T>(*this) -= other;
    }
    Polynomial<T>& operator*=(T const& x) {
        for (T& coeff : coeffs)
            coeff *= x;
        shorten();
        return *this;
    }
    Polynomial<T> operator*(T const& x) {
        return Polynomial<T>(*this) *= x;
    }
    Polynomial<T>& operator/=(T const& x) {
        return *this *= (1 / x);
    }
    Polynomial<T> operator/(T const& x) {
        return Polynomial<T>(*this) /= x;
    }
    Polynomial<T>& operator*=(Polynomial<T> const& other) {
        std::vector<T> new_coeffs(coeffs.size() + other.coeffs.size() - 1, 0);
        for (int i = 0; i < coeffs.size(); i++) {
            for (int j = 0; j < other.coeffs.size(); j++) {
                new_coeffs[i+j] += coeffs[i] * other.coeffs[j];
            }
        }
        coeffs.swap(new_coeffs);
        shorten();
        return *this;
    }
    Polynomial<T> operator*(Polynomial<T> const& other) {
        return Polynomial<T>(*this) *= other;
    }

    T evaluate(T const& x) {  // Horner's method
        T res = 0;
        for (int i = coeffs.size() - 1; i >= 0; i--)
            res = res * x + coeffs[i];
        return res;
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
};
