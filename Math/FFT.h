#include <vector>
#include <complex>
#include <algorithm>

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
    FFT(int max_degree) {
        int lg = get_lg(max_degree + 1);
        reverse.resize(lg + 1);

        int size = 1 << lg;
        ws.resize(lg);
        ws[lg-1].resize(size >> 1);
        for (int i = 0; i < (size >> 1); i++) {
            double ang = 2 * PI * i / size;
            ws[lg-1][i] = {cos(ang), sin(ang)};
        }
        for (int j = lg - 2; j >= 0; j--) {
            int sz_level = ws[j+1].size() >> 1;
            ws[j].resize(sz_level);
            for (int i = 0; i < sz_level; i++)
                ws[j][i] = ws[j+1][i<<1];
        }
    }

    int get_lg(int x) {
        return x ? 32 - __builtin_clz(x - 1) : 0;
    }

    void precompute_reverse(int lg) {
        int size = 1 << lg;
        reverse[lg].assign(size, 0);
        for (int i = 1, j = 0; i < size; i++) {
            int bit = size >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            reverse[lg][i] = j;
        }
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

    template <typename T>
    std::vector<long long> multiply_brute_force(std::vector<T> const& a, std::vector<T> const& b, int sz) {
        std::vector<long long> result(sz);
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)b.size(); j++) {
                result[i + j] += a[i] * b[j];
            }
        }
        return result;
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
    std::vector<long long> multiply(std::vector<T> const& a, std::vector<T> const& b) {
        int result_size = a.size() + b.size() - 1;
        if (result_size <= 200)
            return multiply_brute_force(a, b, result_size);

        vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
        multiply(fa, fb);

        std::vector<long long> result(result_size);
        for (int i = 0; i < result_size; i++)
            result[i] = std::llround(fa[i].real());
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

    std::vector<int> multiply_mod(std::vector<int> const& a, std::vector<int> const& b, int const mod) {
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
