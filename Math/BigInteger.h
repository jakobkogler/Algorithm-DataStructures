#include <complex>
#include <vector>
#include <algorithm>

constexpr int power(int x, int e) {
    return e ? x * power(x, e-1) : 1;
}

class FFT_BigMath {
public:
    using cd = std::complex<double>;
    using vcd = std::vector<cd>;
    using vvcd = std::vector<vcd>;

    static int get_lg(int x) {
        return x ? 32 - __builtin_clz(x - 1) : 0;
    }

    static void precompute(int lg) {
        int size = 1 << lg;

        if ((int)reverse.size() <= lg) {
            reverse.resize(lg + 1);
            ws.resize(lg + 1);
        }

        if (reverse[lg].empty()) {
            reverse[lg].assign(size, 0);
            for (int i = 1, j = 0; i < size; i++) {
                int bit = size >> 1;
                for (; j & bit; bit >>= 1)
                    j ^= bit;
                j ^= bit;
                reverse[lg][i] = j;
            }
        }

        if (ws[lg-1].empty()) {
            ws[lg-1].resize(size >> 1);
            for (int i = 0; i < (size >> 1); i++) {
                double ang = 2 * PI * i / size;
                ws[lg-1][i] = {cos(ang), sin(ang)};
            }
            for (int j = lg - 2; j >= 0 && ws[j].empty(); j--) {
                int sz_level = ws[j+1].size() >> 1;
                ws[j].resize(sz_level);
                for (int i = 0; i < sz_level; i++)
                    ws[j][i] = ws[j+1][i<<1];
            }
        }
    }

    static void fft(vcd & a, bool inv) {
        int lg = get_lg(a.size());
        int size = 1 << lg;
        precompute(lg);

        auto& rev = reverse[lg];
        for (int i = 0; i < size; i++) {
            if (i < rev[i])
                swap(a[i], a[rev[i]]);
        }

        for (int k = 0; k < lg; k++) {
            int len = 1 << k;
            auto& w = ws[k];
            for (int i = 0; i < size; i += 2*len) {
                for (int j = 0; j < len; j++) {
                    cd u = a[i+j], v = a[i+j+len] * w[j];
                    a[i+j] = u + v;
                    a[i+j+len] = u - v;
                }
            }
        }

        if (inv) {
            for (cd & x : a)
                x /= size;
            std::reverse(a.begin() + 1, a.end());
        }
    }

    static void multiply(vcd & fa, vcd & fb) {
        int result_size = fa.size() + fb.size() + 1;
        int size = 1 << get_lg(result_size);
        fa.resize(size);
        fb.resize(size);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < size; i++)
            fa[i] *= fb[i];
        fft(fa, true);

        fa.resize(result_size);
    }

    static std::vector<long long> multiply_brute_force(std::vector<int> const& a, std::vector<int> const& b) {
        std::vector<long long> result(a.size() + b.size() + 1, 0);
        for (auto i = 0u; i < a.size(); i++) {
            for (auto j = 0u; j < b.size(); j++) {
                result[i + j] += a[i] * b[j];
            }
        }
        return result;
    }

    static std::vector<long long> multiply(std::vector<int> const& a, std::vector<int> const& b) {
        std::vector<long long> result;
        int result_size = a.size() + b.size() + 1;
        if (result_size <= 0) {
            result = multiply_brute_force(a, b);
        } else {
            vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
            multiply(fa, fb);
            result.resize(result_size);
            for (int i = 0; i < result_size; i++)
                result[i] = std::llround(fa[i].real());
        }
        return result;
    }

private:
    static std::vector<std::vector<int>> reverse;
    static std::vector<vcd> ws;
    static const double PI;
};

std::vector<std::vector<int>> FFT_BigMath::reverse;
std::vector<FFT_BigMath::vcd> FFT_BigMath::ws;
const double FFT_BigMath::PI = std::acos(-1);

class BigInteger {
public:
    BigInteger(long long x = 0) {
        if (x > 0)
            sign = 1;
        else if (x == 0)
            sign = 0;
        else
            sign = -1;
        x *= sign;

        while (x) {
            data.push_back(x % BASE);
            x /= BASE;
        }
    }

    BigInteger& add(BigInteger const& o, int o_sign) {
        if (sign == o_sign) {
            data.resize(std::max(data.size(), o.data.size()) + 1, 0);
            int carry = 0;
            for (auto i = 0u; i < data.size(); i++) {
                if (i < o.data.size())
                    carry += o.data[i];
                carry += data[i];

                data[i] = carry % BASE;
                carry /= BASE;
            }
        } else if (o_sign == 0) {
            // nothing
        } else if (sign == 0) {
            sign = o_sign;
            data = o.data;
        } else {
            int cmp = compare_abs(o);
            data.resize(std::max(data.size(), o.data.size()) + 1, 0);
            if (cmp == 0) {
                sign = 0;
                data.clear();
            } else if (cmp == 1) {
                int carry = 0;
                for (auto i = 0u; i < data.size(); i++) {
                    carry += data[i];
                    if (i < o.data.size())
                        carry -= o.data[i];

                    if (carry < 0) {
                        carry += BASE;
                        data[i] = carry;
                        carry = 1;
                    } else {
                        data[i] = carry;
                        carry = 0;
                    }
                }
            } else {
                int carry = 0;
                for (auto i = 0u; i < data.size(); i++) {
                    if (i < o.data.size())
                        carry += o.data[i];
                    carry -= data[i];

                    if (carry < 0) {
                        carry += BASE;
                        data[i] = carry;
                        carry = 1;
                    } else {
                        data[i] = carry;
                        carry = 0;
                    }
                }
                sign = o_sign;
            }
        }

        pop_zeros();
        return *this;
    }

    BigInteger& operator+=(BigInteger const& o) {
        return add(o, o.sign);
    }

    BigInteger operator+(BigInteger const& o) const {
        BigInteger t = *this;
        t += o;
        return t;
    }

    BigInteger& operator-=(BigInteger const& o) {
        if (o.sign)
            return add(o, -o.sign);
        return *this;
    }

    BigInteger operator-(BigInteger const& o) const {
        BigInteger t = *this;
        t -= o;
        return t;
    }

    BigInteger& operator*=(long long o) {
        return *this *= BigInteger(o);
    }

    BigInteger operator*(long long o) {
        return *this * BigInteger(o);
    }

    BigInteger& operator*=(int o) {
        if (o == 0) {
            sign = 0;
            data.clear();
        } else {
            if (o < 0) {
                sign *= -1;
                o = std::abs(o);
            }
            int n = data.size();
            data.resize(n + 9, 0);
            long long carry = 0;
            for (int i = 0; i < n || carry; i++) {
                carry += (long long)data[i] * o;
                data[i] = carry % BASE;
                carry /= BASE;
            }
            pop_zeros();
        }
        return *this;
    }

    BigInteger operator*(int o) const {
        BigInteger t = *this;
        t *= o;
        return t;
    }

    BigInteger& operator*=(BigInteger const& o) {
        sign *= o.sign;
        if (sign == 0) {
            data.clear();
        } else {
            auto result = FFT_BigMath::multiply(data, o.data);
            data.resize(result.size());
            long long carry = 0;
            for (auto i = 0u; i < result.size(); i++) {
                carry += result[i];
                data[i] = carry % BASE;
                carry /= BASE;
            }
            pop_zeros();
        }
        return *this;
    }

    BigInteger operator*(BigInteger const& o) const {
        BigInteger t = *this;
        t *= o;
        return t;
    }

    BigInteger& operator/=(int d) {
        long long remainder = 0;
        for (int i = data.size() - 1; i >= 0; i--) {
            remainder = BASE * remainder + data[i];
            data[i] = remainder / d;
            remainder %= d;
        }
        pop_zeros();
        return *this;
    }

    BigInteger operator/(int d) {
        BigInteger b = *this;
        b /= d;
        return b;
    }

    void pop_zeros() {
        while (!data.empty() && data.back() == 0)
            data.pop_back();
    }

    friend std::ostream& operator<<(std::ostream &stream, BigInteger const& b) {
        if (b.data.empty()) {
            stream << 0;
        } else {
            if (b.sign == -1)
                stream << '-';
            stream << b.data.back();
            for (int i = b.data.size() - 2; i >= 0; i--) {
                stream.width(DIGITS);
                stream.fill('0');
                stream << b.data[i];
            }
        }
        return stream;
    }

    friend std::istream& operator>>(std::istream& is, BigInteger& b) {
        std::string s;
        is >> s;
        int start = 0;
        if (s == "0") {
            b.sign = 0;
            b.data.clear();
        } else {
            if (s[0] == '-') {
                b.sign = -1;
                start++;
            } else {
                b.sign = 1;
            }
            b.data.resize((s.size() - start + DIGITS - 1) / DIGITS);
            for (int i = 0, idx = s.size() - 1; i < (int)b.data.size(); i++, idx -= DIGITS) {
                int value = 0;
                for (int j = std::max(start, idx - DIGITS + 1); j <= idx; j++)
                    value = value * 10 + s[j] - '0';
                b.data[i] = value;
            }
        }
        return is;
    }
          
    unsigned int digits() const {
        if (data.empty())
            return 0;
        unsigned int d = (data.size() - 1) * DIGITS;
        int x = data.back();
        while (x > 0) {
            d++;
            x /= 10;
        }
        return d;
    }

    int compare_abs(BigInteger const& o) const {
        if (data.size() != o.data.size())
            return data.size() < o.data.size() ? -1 : 1;
        for (int i = data.size() - 1; i >= 0; i--) {
            if (data[i] != o.data[i])
                return data[i] < o.data[i] ? -1 : 1;
        }
        return 0;
    }
    int compare(BigInteger const& o) const {
        if (sign < o.sign)
            return -1;
        if (sign > o.sign)
            return 1;
        int abs = compare_abs(o);
        return (abs != 0 && sign == -1) ? -abs : abs;
    }
    bool operator==(BigInteger const& o) const { return compare(o) == 0; }
    bool operator!=(BigInteger const& o) const { return compare(o) != 0; }
    bool operator<(BigInteger const& o) const { return compare(o) == -1; }
    bool operator<=(BigInteger const& o) const { return compare(o) <= 0; }
    bool operator>(BigInteger const& o) const { return compare(o) == 1; }
    bool operator>=(BigInteger const& o) const { return compare(o) >= 0; }

private:
    static const int DIGITS = 4;
    static const int BASE = power(10, DIGITS);

    int sign;
    std::vector<int> data;
};

BigInteger power(BigInteger base, int e) {
    BigInteger result = 1;
    while (e) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
