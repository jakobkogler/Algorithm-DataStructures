bool isPrime(int x) {
    if (x == 1) return false;
    if (x == 2)
        return true;
    if (x % 2 == 0)
        return false;
    for (int div = 3; div * div <= x; div+=2) {
        if (x % div == 0) return false;
    }
    return true;
}
