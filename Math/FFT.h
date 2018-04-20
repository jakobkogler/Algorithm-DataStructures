#include <vector>
#include <complex>

class FFT {
public:
    FFT(int max_degree) {
        size = 1;
        lg = 0;
        while (size < max_degree + 1) {
            size <<= 1;
            lg++;
        }

        reverse.assign(size, 0);
        for (int i = 1, j = 0; i < size; i++) {
            int bit = size >> 1;
            for (; j & bit; bit >>= 1)
                j ^= bit;
            j ^= bit;
            reverse[i] = j;
        }
    }

    using cd = std::complex<double>;
    using vcd = std::vector<cd>;

    void fft(vcd & a, bool invert) {
        for (int i = 0; i < size; i++) {
            if (i < reverse[i])
                swap(a[i], a[reverse[i]]);
        }

        for (int len = 1; len < size; len <<= 1) {
            double ang = PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < size; i += 2*len) {
                cd w(1);
                for (int j = 0; j < len; j++) {
                    cd u = a[i+j], v = a[i+j+len] * w;
                    a[i+j] = u + v;
                    a[i+j+len] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (cd & x : a)
                x /= size;
        }
    }

    std::vector<int> multiply(std::vector<int> const& a, std::vector<int> const& b) {
        vcd fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(size);
        fb.resize(size);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < size; i++)
            fa[i] *= fb[i];
        fft(fa, true);

        std::vector<int> result(size);
        for (int i = 0; i < size; i++)
            result[i] = round(fa[i].real());
        return result;
    }

private:
    int size;
    int lg;
    std::vector<int> reverse;
    double PI = acos(-1);
};
