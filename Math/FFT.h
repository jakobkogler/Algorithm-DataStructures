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
    std::vector<long long> multiply(std::vector<T> const& a, std::vector<T> const& b) {
        int sz = a.size() + b.size() - 1;
        vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(size);
        fb.resize(size);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < size; i++)
            fa[i] *= fb[i];
        fft(fa, true);

        std::vector<long long> result(sz);
        for (int i = 0; i < sz; i++)
            result[i] = std::llround(fa[i].real());
        return result;
    }

private:
    int size;
    int lg;
    std::vector<int> reverse;
    std::vector<vcd> ws;
};
