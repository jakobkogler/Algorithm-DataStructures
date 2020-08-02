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
        for (int i = 0; i < (int)other.coeffs.size(); i++) {
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
        for (int i = 0; i < (int)other.coeffs.size(); i++) {
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
        return divide(other).first;
    }

    Polynomial<T>& operator/=(Polynomial<T> const& other) {
        auto res = divide(other).first;
        coeffs.swap(res.coeffs);
        return *this;
    }

    Polynomial<T> operator%(Polynomial<T> const& other) const {
        return divide(other).second;
    }

    Polynomial<T>& operator%=(Polynomial<T> const& other) {
        auto res = divide(other).second;
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
        for (int i = 1; i < (int)cpy.size(); i++) {
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
        if ((int)coeffs.size() <= n)
            return *this;
        Polynomial<T> ret({coeffs.begin(), coeffs.begin() + n});
        ret.shorten();
        return ret;
    }

    /**
     * Compute the product of the linear factors (x - r[0]) * (x - r[1]) * ...
     * using binary splitting in O(n log(n)^2)
     */
    static Polynomial<T> linear_factors_product(std::vector<T> const& r) {
        return linear_factors_product(r, 0, r.size());
    }

    /**
     * Evaluate the polynomial at multiple points in O(n log(n)^2)
     */
    std::vector<T> multi_point_evaluation(std::vector<T> const& x) {
        int n = x.size();
        std::vector<Polynomial<T>> tree(4*n);
        linear_factors_product(x, tree, 1, 0, n);
        return multi_point_evaluation(x, tree, 1, 0, n);
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

    /**
     * extracts the coefficients from l to r
     */
    Polynomial<T> substr(int l, int r) const {
        l = std::min(l, (int)coeffs.size());
        r = std::min(r, (int)coeffs.size());
        return Polynomial<T>(std::vector<T>(coeffs.begin() + l, coeffs.begin() + r));
    }

    /**
     * multiplies the polynomial with x^n
     */
    Polynomial<T> operator<<(int n) const {
        auto cpy = *this;
        cpy.coeffs.insert(cpy.coeffs.begin(), n, 0);
        return cpy;
    }

    /**
     * Computes the reciprocal polynomial modulo x^n in O(n log(n))
     */
    Polynomial<T> reciprocal(int n) const {
        assert(coeffs[0] != 0);
        int sz = 1;
        Polynomial<T> R({T(1) / coeffs[0]});
        while (sz < n) {
            // trick from adamant
            auto C = (R * (*this % (3 * sz))).substr(sz, 2 * sz);
            R -= (R * C % sz) << sz;
            sz *= 2;
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
     * divide this polynomial by g in O(n log(n))
     * returns quotient and remainder
     */
    std::pair<Polynomial<T>, Polynomial<T>> divide(Polynomial<T> const& g) const {
        if (deg() < g.deg())
            return {Polynomial({T(0)}), *this};
        int n = deg() - g.deg() + 1;
        if (min(n, g.deg()) < 500)
            return divide_brute_force(g);

        auto quotient = ((rev() % n) * (g.rev() % n).reciprocal(n) % n).rev();
        return {quotient, *this - quotient * g};
    }

    /**
     * Normal division from school
     * returns quotient and remainder
     */
    std::pair<Polynomial<T>, Polynomial<T>> divide_brute_force(Polynomial<T> const& g) const {
        std::vector<T> divident(coeffs);
        const auto& divisor = g.coeffs;
        std::vector<T> quotient;
        quotient.reserve(divident.size() - divisor.size() + 1);
        while (divident.size() >= divisor.size()) {
            T q = divident.back() / divisor.back();
            quotient.push_back(q);
            for (int i = 0; i < (int)divisor.size(); i++) {
                divident[divident.size() - i - 1] -= q * divisor[divisor.size() - i - 1];
            }
            divident.pop_back();
        }
        std::reverse(quotient.begin(), quotient.end());
        return {Polynomial<T>(quotient), Polynomial<T>(divident)};
    }

    /**
     * Compute the product of the linear factors (x - r[0]) * (x - r[1]) * ...
     * using binary splitting in O(n log(n)^2)
     */
    static Polynomial<T> linear_factors_product(std::vector<T> const& roots, int l, int r) {
        if (l + 1 == r)
            return Polynomial<T>({-roots[l], 1});
        int m = (l + r) / 2;
        return linear_factors_product(roots, l, m) * linear_factors_product(roots, m, r);
    }

    /**
     * Compute the product of the linear factors (x - r[0]) * (x - r[1]) * ...
     * using binary splitting in O(n log(n)^2) and store the tree
     */
    static Polynomial<T> linear_factors_product(std::vector<T> const& roots, std::vector<Polynomial<T>>& tree, int v, int l, int r) {
        if (l + 1 == r)
            return tree[v] = Polynomial<T>({-roots[l], 1});
        int m = (l + r) / 2;
        return tree[v] = linear_factors_product(roots, tree, 2*v, l, m) * linear_factors_product(roots, tree, 2*v+1, m, r);
    }

    /**
     * Compute the product of the linear factors (x - r[0]) * (x - r[1]) * ...
     * using binary splitting in O(n log(n)^2) and store the tree
     */
    std::vector<T> multi_point_evaluation(std::vector<T> const& x, std::vector<Polynomial<T>> const& tree, int v, int l, int r) const {
        if (l + 1 == r)
            return {evaluate(x[l])};

        int m = (l + r) / 2;
        std::vector<T> res1 = (*this % tree[2*v]).multi_point_evaluation(x, tree, 2*v, l, m);
        auto res2 = (*this % tree[2*v+1]).multi_point_evaluation(x, tree, 2*v+1, m, r);
        res1.insert(res1.end(), res2.begin(), res2.end());
        return res1;
    }
};

/**
 * Computes the determinant of the Vandermonde Matrix
 * This is equivalent to the product Π_{i!=j} (a_j - a_i)
 * or the square of Π_{i<j} (a_j - a_i)
 */
template <typename T>
T VandermondeDeterminant(std::vector<T> const& v) {
    auto P = Polynomial<T>::linear_factors_product(v);
    auto evaled = P.derivation().multi_point_evaluation(v);

    T prod(1);
    for (auto x : evaled)
        prod *= x;
    int n = v.size();
    if ((long long)n * (n - 1) / 2 % 2)
        prod *= -1;
    return prod;
}
