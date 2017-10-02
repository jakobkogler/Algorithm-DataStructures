#include <bits/stdc++.h>
using namespace std;

class WaveletTree
{
public:
    WaveletTree(vector<int> &v, int size) : s(size), nodes(2*size) {
        build(v.begin(), v.end(), 0, s-1, 1);
    }

    // l, r is 0 based
    // k is 1 based
    // range [l, r)
    int kTh(int l, int r, int k) {
        int idx = 1;
        int L = 0, U = s-1;
        while (L != U) {
            int M = (L + U) / 2;
            int ll = nodes[idx][l];
            int lr = nodes[idx][r];
            if (k <= lr - ll) {
                l = ll, r = lr, U = M, idx = 2*idx;
            } else {
                k -= lr - ll, l -= ll, r -= lr, L = M + 1, idx = 2*idx+1;
            }
        }
        return L;
    }

private:
    int s;
    vector<vector<int>> nodes;

    using iter = vector<int>::iterator;

    void build(iter b, iter e, int L, int U, int idx) {
        if (L == U)
            return;
        int M = (L + U) / 2;

        nodes[idx].resize(distance(b, e) + 1);
        nodes[idx][0] = 0;
        int i = 0;
        for (iter it = b; it != e; ++it) {
            nodes[idx][i+1] = nodes[idx][i] + (*it <= M);
            i++;
        }

        iter p = stable_partition(b, e, [M](int i){ return i <= M; });

        build(b, p, L, M, idx*2);
        build(p, e, M+1, U, idx*2+1);
    }
};

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
