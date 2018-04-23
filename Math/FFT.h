#include <vector>
#include <complex>
#include <algorithm>

class FFT {
public:
    FFT(int max_degree) {
        size = 1;
        lg = 0;
        while (size < max_degree + 1) {
            size <<= 1;
            lg++;
        }
        precompute();
    }

    void precompute() {
        reverse.assign(size, 0);
        for (int i = 1, j = 0; i < size; i++) {
            int bit = size >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            reverse[i] = j;
        }

        const double PI = acos(-1);
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

    using cd = std::complex<double>;
    using vcd = std::vector<cd>;

    void fft(vcd & a, bool inv) {
        for (int i = 0; i < size; i++) {
            if (i < reverse[i])
                swap(a[i], a[reverse[i]]);
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

    template <typename T>
    std::vector<long long> multiply(std::vector<T> const& a, std::vector<T> const& b) {
        int result_size = a.size() + b.size() - 1;
        if (result_size <= 200)
            return multiply_brute_force(a, b, result_size);

        vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(size);
        fb.resize(size);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < size; i++)
            fa[i] *= fb[i];
        fft(fa, true);

        std::vector<long long> result(result_size);
        for (int i = 0; i < result_size; i++)
            result[i] = std::llround(fa[i].real());
        return result;
    }

    std::vector<int> multiply_mod(std::vector<int> const& a, std::vector<int> const& b, int const mod) {
        int result_size = a.size() + b.size() - 1;

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
            cd a = (v1[i] + conj(v1[j])) * cd(0.5, 0);
            cd b = (v1[i] - conj(v1[j])) * cd(0, -0.5);
            cd c = (v2[i] + conj(v2[j])) * cd(0.5, 0);
            cd d = (v2[i] - conj(v2[j])) * cd(0, -0.5);
            r1[i] = a*c + a*d*cd(0,1);
            r2[i] = b*c + b*d*cd(0,1);
        }
        fft(r1, true);
        fft(r2, true);

        std::vector<int> result(result_size);
        for (int i = 0; i < result_size; i++) {
            long long a = std::llround(r1[i].real()) % mod;
            long long b = std::llround(r1[i].imag() + r2[i].real()) % mod;
            long long c = std::llround(r2[i].imag()) % mod;
            result[i] = ((a << 30) + (b << 15) + c) % mod;
        }
        return result;
    }

private:
    int size;
    int lg;
    std::vector<int> reverse;
    std::vector<vcd> ws;
};
