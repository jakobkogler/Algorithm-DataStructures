#pragma once
#include <vector>
#include <complex>
#include <algorithm>
#include "Modular.h"
#include "MontgomeryModular.h"

template <typename T>
void transpose(std::vector<std::vector<T>> & M) {
    auto tmp = M;
    M.resize(tmp[0].size());
    for (auto& row : M)
        row.resize(tmp.size());

    for (int i = 0; i < (int)M.size(); i++) {
        for (int j = 0; j < (int)M[0].size(); j++) {
            M[i][j] = tmp[j][i];
        }
    }
}

class FFT {
public:
    FFT(int max_degree=1) {
        init(max_degree);
    }

    void init(int max_degree) {
        int lg = get_lg(max_degree + 1);
        reverse.resize(lg + 1);

        int size = 1 << lg;
        ws.resize(lg);
        ws[0] = {{1, 0}};
        for (int j = 1; j < lg; j++) {
            int sz_level = 1 << j;
            ws[j].reserve(sz_level);
            double angle = PI / sz_level;
            cd z(cos(angle), sin(angle));
            for (const auto x : ws[j-1]) {
                ws[j].push_back(x);
                ws[j].push_back(x * z);
            }
        }
    }

    int get_lg(int x) {
        return x ? 32 - __builtin_clz(x - 1) : 0;
    }

    void precompute_reverse(int lg) {
        int size = 1 << lg;
        auto& rev = reverse[lg];
        rev.assign(size, 0);
        for (int i = 1; i < size; i++)
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (lg - 1));
    }

    using cd = std::complex<double>;
    using vcd = std::vector<cd>;
    using vvcd = std::vector<vcd>;

    void fft(vcd & a, bool inv) {
        int lg = get_lg(a.size());
        if (reverse[lg].empty())
            precompute_reverse(lg);
        int size = 1 << lg;

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

    void multiply(vcd & fa, vcd & fb) {
        int result_size = fa.size() + fb.size() - 1;
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

    template <typename T>
    std::vector<T> multiply(std::vector<T> const& a, std::vector<T> const& b) {
        int result_size = a.size() + b.size() - 1;
        vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
        multiply(fa, fb);
        fa.resize(result_size);
        return convert_back<T>(fa);
    }

    template <typename T>
    std::vector<T> convert_back(vcd const& v) {
        std::vector<T> result(v.size());
        for (int i = 0; i < v.size(); i++)
            result[i] = v[i].real();
        return result;
    }

    void fft2D(vvcd & a, bool inv) {
        for (auto& row : a)
            fft(row, inv);
        transpose(a);
        for (auto& row : a)
            fft(row, inv);
        transpose(a);
    }

    void multiply2D(vvcd & a, vvcd & b) {
        int result_r = a.size() + b.size() - 1;
        int result_c = a[0].size() + b[0].size() - 1;
        int size_r = 1 << get_lg(result_r);
        int size_c = 1 << get_lg(result_c);
        a.resize(size_r);
        b.resize(size_r);
        for (auto& row : a)
            row.resize(size_c);
        for (auto& row : b)
            row.resize(size_c);

        fft2D(a, false);
        fft2D(b, false);
        for (int i = 0; i < size_r; i++) {
            for (int j = 0; j < size_c; j++) {
                a[i][j] *= b[i][j];
            }
        }
        fft2D(a, true);

        a.resize(result_r);
        for (auto& row : a)
            row.resize(result_c);
    }

    template<typename T>
    std::vector<T> multiply_mod(std::vector<T> const& a, std::vector<T> const& b, int const mod) {
        int result_size = a.size() + b.size() - 1;
        int size = 1 << get_lg(result_size);

        vcd v1(size), v2(size);
        for (int i = 0; i < (int)a.size(); i++)
            v1[i] = cd(a[i] >> 15, a[i] & 32767);
        for (int i = 0; i < (int)b.size(); i++)
            v2[i] = cd(b[i] >> 15, b[i] & 32767);
        fft(v1, false);
        fft(v2, false);

        vcd r1(size), r2(size);
        for (int i = 0; i < size; i++) {
            int j = i ? size - i : i;
            cd ans1 = (v1[i] + conj(v1[j])) * cd(0.5, 0);
            cd ans2 = (v1[i] - conj(v1[j])) * cd(0, -0.5);
            cd ans3 = (v2[i] + conj(v2[j])) * cd(0.5, 0);
            cd ans4 = (v2[i] - conj(v2[j])) * cd(0, -0.5);
            r1[i] = ans1*ans3 + ans1*ans4*cd(0,1);
            r2[i] = ans2*ans3 + ans2*ans4*cd(0,1);
        }
        fft(r1, true);
        fft(r2, true);

        std::vector<int> result(result_size);
        for (int i = 0; i < result_size; i++) {
            long long ans1 = std::llround(r1[i].real()) % mod;
            long long ans2 = std::llround(r1[i].imag() + r2[i].real()) % mod;
            long long ans3 = std::llround(r2[i].imag()) % mod;
            result[i] = ((ans1 << 30) + (ans2 << 15) + ans3) % mod;
        }
        return result;
    }

private:
    std::vector<std::vector<int>> reverse;
    std::vector<vcd> ws;
    const double PI = std::acos(-1);
};

template <>
std::vector<long long> FFT::convert_back(vcd const& v) {
    std::vector<long long> result(v.size());
    for (int i = 0; i < v.size(); i++)
        result[i] = std::llround(v[i].real());
    return result;
}

template <>
std::vector<int> FFT::convert_back(vcd const& v) {
    std::vector<int> result(v.size());
    for (int i = 0; i < v.size(); i++)
        result[i] = std::round(v[i].real());
    return result;
}

FFT fft;

template <typename T>
std::vector<T> from_int_vector(std::vector<int> const& v) {
    return std::vector<T>(v.begin(), v.end());
}

template <typename T>
std::vector<int> to_int_vector(std::vector<T> const& v) {
    std::vector<int> w;
    w.reserve(v.size());
    for (int i = 0; i < v.size(); i++)
        w[i] = v[i].value;
    return w;
}

template <typename T>
std::vector<T> fft_multiply(std::vector<T> const& a, std::vector<T> const& b) {
    return fft.multiply(a, b);
}

template <int MOD>
std::vector<Modular<MOD>> fft_multiply(std::vector<Modular<MOD>> const& a, std::vector<Modular<MOD>> const& b) {
    return from_int_vector<Modular<MOD>>(fft.multiply_mod(to_int_vector(a), to_int_vector(b), MOD));
}

template <int MOD>
std::vector<MontgomeryModular<MOD>> fft_multiply(std::vector<MontgomeryModular<MOD>> const& a, std::vector<MontgomeryModular<MOD>> const& b) {
    return from_int_vector<MontgomeryModular<MOD>>(fft.multiply_mod(to_int_vector(a), to_int_vector(b), MOD));
}
