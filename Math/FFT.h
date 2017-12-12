#include <vector>
#include <complex>

namespace FFT 
{
    using cd = std::complex<double>;
    using vcd = std::vector<cd>;

    void prepair(vcd& p) {
        int n = 1;
        while (n < (int)p.size()) {
            n *= 2;
        }

        while ((int)p.size() < n) {
            p.push_back(0);
        }
    }
    
    vcd fft(const vcd& as) {
        int n = as.size();
        int k = 0;
        while ((1 << k) < n) k++;
        std::vector<int> rev(n);
        rev[0] = 0;
        int high1 = -1;
        for (int i = 1; i < n; i++) {
            if ((i & (i - 1)) == 0)
                high1++;
            rev[i] = rev[i ^ (1 << high1)];
            rev[i] |= 1 << (k - high1 - 1);
        }

        vcd roots(n);
        for (int i = 0; i < n; i++) {
            double alpha = 2 * M_PI * i / n;
            roots[i] = cd(cos(alpha), sin(alpha));
        }

        vcd cur(n);
        for (int i = 0; i < n; i++) {
            cur[i] = as[rev[i]];
        }

        for (int len = 1; len < n; len <<= 1) {
            vcd ncur(n);
            int rstep = roots.size() / (len * 2);
            for (int pdest = 0; pdest < n;) {
                int p1 = pdest;
                for (int i = 0; i < len; i++) {
                    cd val = roots[i * rstep] * cur[p1 + len];
                    ncur[pdest] = cur[p1] + val;
                    ncur[pdest + len] = cur[p1] - val;
                    pdest++, p1++;
                }
                pdest += len;
            }
            cur.swap(ncur);
        }
        return cur;
    }

    vcd inverse_fft(const vcd& p) {
        int n = p.size();
        auto res = fft(p);
        vcd inverse(n);
        inverse[0] = res[0] / (double)n;
        for (int i = 1; i < n; i++) {
            inverse[i] = res[n - i] / (double)n;
        }
        return inverse;
    }

    vcd multiply(const vcd& a, const vcd& b) {
        auto c = a;
        for (int i = 0; i < (int)c.size(); i++) {
            c[i] *= b[i];
        }
        return c;
    }
};
