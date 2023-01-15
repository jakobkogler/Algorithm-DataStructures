template <typename T>
T floor_div(T a, long long b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }
    return (a < 0 ? a - (b - 1) : a) / b;
}

template <typename T>
T ceil_div(T a, long long b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }
    return (a >= 0 ? a + (b - 1) : a) / b;
}
