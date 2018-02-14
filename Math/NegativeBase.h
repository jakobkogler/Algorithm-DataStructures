#include <vector>

std::vector<int> to_negative_base(int x, int pos_base) {
    std::vector<int> coeff;
    while (x) {
        int rem = x % pos_base;
        if (rem < 0) rem += pos_base;
        coeff.push_back(rem);
        x -= rem;
        x /= -pos_base;
    }
    return coeff;
}
