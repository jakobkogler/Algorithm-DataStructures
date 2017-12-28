int totient(int num)
{
    int res = num;
    for (int div = 2; div * div <= num; div++) {
        if (num % div == 0) {
            res /= div;
            res *= div - 1;
            while (num % div == 0) {
                num /= div;
            }
        }
    }
    if (num > 1) {
        res /= num;
        res *= num - 1;
    }
    return res;
}
