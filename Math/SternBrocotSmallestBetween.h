#include "Fraction.h"

Fraction mediant(Fraction const& f, Fraction const& g, int wf=1, int wg=1) {
    return {f.a*wf + g.a*wg, f.b*wf + g.b*wg};
}

Fraction find_smallest_between(Fraction const& L, Fraction const& R) {
    Fraction l(0, 1), r(1, 0);

    while (true) {
        Fraction m = mediant(l, r);
        if (m <= L) {  // move left border to the right, but don't overshoot
            long long bl = 0, br = 1;
            while (mediant(l, r, 1, br) <= L)
                br *= 2;
            while (bl + 1 != br) {
                long long bm = (bl + br) / 2;
                if (mediant(l, r, 1, bm) <= L)
                    bl = bm;
                else
                    br = bm;
            }
            l = mediant(l, r, 1, bl);
        } else if (m >= R) {  // move right border to the left, but don't overshoot
            long long bl = 0, br = 1;
            while (mediant(l, r, br, 1) >= R)
                br *= 2;
            while (bl + 1 != br) {
                long long bm = (bl + br) / 2;
                if (mediant(l, r, bm, 1) >= R)
                    bl = bm;
                else
                    br = bm;
            }
            r = mediant(l, r, bl, 1);
        } else {
            return m;
        }
    }
}
