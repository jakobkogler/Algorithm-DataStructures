#include <iostream>
#include "PrimeLargest.h"

std::vector<int> largest;

int totient(int num)
{
    int coprime = 1;
    while (num > 1) {
        int l = largest[num];
        num /= l;
        int p = 1;
        while (largest[num] == l) {
            num /= l;
            p *= l;
        }
        coprime *= p * (l - 1);
    }
    return coprime;
}

int main()
{
    computePrimesLargest(1e7, largest);
    std::cout << totient(99) << std::endl;
}
