template <typename T>
T geometric_series(T a, T r, int n) {
    if (r != 1)
        return a * (1 - power(r, n)) / (1 - r);
    else
        return a * n;
}
