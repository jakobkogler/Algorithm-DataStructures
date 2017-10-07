#include <bits/stdc++.h>

int dp[21][4001];

int f(int l, int r);

void solve(int k, int l, int r, int optl, int optr) {
    if (l > r) return;
    int m = (l + r) >> 1;
    int opt = optl;
    int best_value = std::numeric_limits<int>::max();
    for (int i = optl; i <= std::min(optr, m); i++) {
        int value = dp[k-1][i-1] + f(i, m);
        if (value < best_value) {
            opt = i;
            best_value = value;
        }
    }
    dp[k][m] = best_value;
    solve(k, l, m-1, optl, opt);
    solve(k, m+1, r, opt, optr);
}

int main() {
    int n = 4000, K = 20;
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++)
        dp[0][i] = std::numeric_limits<int>::max() / 2;

    for (int k = 1; k <= K; k++) {
        solve(k, 1, n, 1, n);
    }
}
