#include <vector>

void compute_moebius(int n, std::vector<int> &moebius)
{
    moebius.assign(n + 1, 2);
    std::vector<int> divisors(n + 1, 0);

    for (int i = 2; i <= n; i++) {
        if (divisors[i] == 0) {
            for (int j = 1, jm = 1; j * i <= n; j++, jm++) {
                if (jm == i) {
                    jm = 0;
                    moebius[j*i] = 0;
                }
                divisors[j*i]++;
            }
        }
    }

    for (int i = 0; i <= n; i++) {
        if (moebius[i] == 2)
            moebius[i] = (divisors[i] & 1) ? -1 : 1;
    }
}
