#include "../Other/Bitset.h"

int compute_rank(std::vector<Bitset> bitsets) {
    int rank = 0;
    for (int i = 0; i < (int)bitsets.size(); i++) {
        int idx = bitsets[i].first_set();
        if (idx == -1)
            break;
        rank++;
        for (int j = i + 1; j < (int)bitsets.size(); j++) {
            if (bitsets[j].check(idx))
                bitsets[j] ^= bitsets[i];
        }
    }
    return rank;
}
