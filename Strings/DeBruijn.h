#include <string>

class DeBruijn {
    std::string sequence;
    std::string a;
    int k, n;
public:
    // de Bruijn sequence for alphabet k
    // and subsequences of length n.
    DeBruijn(int k, int n) : k(k), n(n) {}

    // returns a cyclic string
    std::string operator()() {
        a.assign(k * n, 'a');
        sequence = "";
        sequence.reserve(power(k, n));

        db(1, 1);
        return sequence;
    }

private:
    void db(int t, int p) {
        if (t > n) {
            if (n % p == 0)
                sequence += a.substr(1, p);
        } else {
            a[t] = a[t - p];
            db(t + 1, p);
            for (char c = a[t - p] + 1; c < 'a' + k; c++) {
                a[t] = c;
                db(t + 1, t);
            }
        }
    }

    int power(int x, int e) {
        return e ? x * power(x, e-1) : 1;
    }
};
