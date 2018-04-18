template <typename T>
T arithmetic_series(T a, T d, int n) {
    return (a*2 + d * (n-1)) * n / 2;
}
