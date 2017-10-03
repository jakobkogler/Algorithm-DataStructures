#include <bits/stdc++.h>
#include "WaveletTree.h"
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    set<int> s(v.begin(), v.end());
    vector<int> w(s.begin(), s.end());
    for (auto &e : v) {
        e = lower_bound(w.begin(), w.end(), e) - w.begin();
    }

    WaveletTree wt(v, w.size());
    while (m --> 0) {
        int l, r, k;
        cin >> l >> r >> k;
        int x = wt.kTh(l-1, r, k);
        cout << w[x] << endl;
    }
}
