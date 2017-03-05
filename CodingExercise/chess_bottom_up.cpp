#include <iostream>
using namespace std;

int main() {
    bool states[15][15] = {{false}};
    for (int diag = 0; diag < 29; diag++) {
        int x = diag < 15 ? diag : 14;
        int y = diag < 15 ? 0 : diag - 14;

        for (; x >= 0 && y < 15; x--, y++) {
            bool state = false;
            if (x - 2 >= 0 && y + 1 < 15 && states[x-2][y+1] == false)
                state = true;
            if (x - 2 >= 0 && y - 1 >= 0 && states[x-2][y-1] == false)
                state = true;
            if (x + 1 < 15 && y - 2 >= 0 && states[x+1][y-2] == false)
                state = true;
            if (x - 1 >= 0 && y - 2 >= 0 && states[x-1][y-2] == false)
                state = true;
            states[x][y] = state;
        }
    }

    int T;
    cin >> T;
    while (T --> 0) {
        int x, y;
        cin >> x >> y;
        cout << (states[x-1][y-1] ? "First" : "Second") << endl;
    }
}
