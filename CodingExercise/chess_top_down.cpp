#include <iostream>
#include <map>
using namespace std;

map<pair<int, int>, bool> dp;

bool compute_state(int x, int y) {
    if (dp.count({x, y}))
        return dp[{x, y}];

    bool state = false;
    if (x - 2 >= 0 && y + 1 < 15 && compute_state(x-2, y+1) == false)
        state = true;
    if (x - 2 >= 0 && y - 1 >= 0 && compute_state(x-2, y-1) == false)
        state = true;
    if (x + 1 < 15 && y - 2 >= 0 && compute_state(x+1, y-2) == false)
        state = true;
    if (x - 1 >= 0 && y - 2 >= 0 && compute_state(x-1, y-2) == false)
        state = true;
    return dp[{x, y}] = state;
}

int main() {
    int T;
    cin >> T;
    while (T --> 0) {
        int x, y;
        cin >> x >> y;
        cout << (compute_state(x-1, y-1) ? "First" : "Second") << endl;
    }
}
