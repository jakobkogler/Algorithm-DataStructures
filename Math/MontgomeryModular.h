#pragma once
#include <cstdint>
#include <utility>

template <int MOD>
class MontgomeryModular {
public:
    using u64 = uint64_t;
    using u32 = uint32_t;
    using i32 = int32_t;
    using MM = MontgomeryModular<MOD>;

    MontgomeryModular(u32 value=0) : value(mult(value, r2)) {}

    MM& operator+=(MM const& other) {
        value += other.value;
        value -= (((i32)mod - 1 - (i32)value) >> 31) & (i32)mod;
        return *this;
    }
    MM operator+(MM const& other) const {
        MM cpy = *this;
        return cpy += other;
    }
    MM& operator-=(MM const& other) {
        value += mod - other.value;
        value -= (((i32)mod - 1 - (i32)value) >> 31) & (i32)mod;
        return *this;
    }
    MM operator-(MM const& other) const {
        MM cpy = *this;
        return cpy -= other;
    }
    MM& operator*=(MM const& other) {
        value = mult(value, other.value);
        return *this;
    }
    MM operator*(MM const& other) const {
        MM cpy = *this;
        return cpy *= other;
    }
    MM& operator/=(MM const& other) {
        return *this *= other.inverse();
    }
    MM operator/(MM const& other) {
        MM cpy = *this;
        return cpy *= other.inverse();
    }
    MM inverse() const {
        MM x;
        x.value = phase2(reduce(value));
        return x;
    }

    u32 normal() const {
        return reduce(value);
    }

    friend std::ostream& operator<<(std::ostream& os, MontgomeryModular<MOD> const& mm) {
        return os << mm.normal();
    }

    bool operator==(MontgomeryModular<MOD> const& mm) const {
        return value == mm.value;
    }
    
    bool operator!=(MontgomeryModular<MOD> const& mm) const {
        return value != mm.value;
    }

    static MM init_inverse(u32 a) {
        MM x;
        x.value = phase2(a);
        return x;
    }

    u32 value;
    static const u32 mod = MOD;

private:
    u32 reduce(u64 x) const {
        u32 xlow = x;
        u32 xhigh = x >> 32;
        u32 q = xlow * inv;
        i32 a = xhigh - ((u64(q) * mod) >> 32);
        a += mod;
        a -= (((i32)mod - 1 - (i32)a) >> 31) & (i32)mod;
        return a;
    }

    u32 mult(u32 a, u32 b) const {
        return reduce(u64(a) * b);
    }

    static constexpr u32 compute_inv(u32 mod) {
        u32 inv = 1;
        for (int i = 0; i < 5; i++)
            inv *= 2 - mod * inv;
        return inv;
    }

    static constexpr u32 compute_r2(u32 mod) {
        u32 r2 = 1;
        for (int i = 0; i < 64; i++) {
            r2 <<= 1;
            if (r2 >= mod)
                r2 -= mod;
        }
        return r2;
    }
    
    static const u32 inv = compute_inv(MOD);
    static const u32 r2 = compute_r2(MOD);

    // Algorithm A from paper "Improvement to Montgomery Modular Inverse Algorithm"
    static std::pair<u32, int> phase1(u32 a) {
        auto p = mod;
        auto u = p;
        auto v = a;
        auto r = 0;
        auto s = 1;
        auto k = 0;
        while (v > 0) {
            if ((u & 1) == 0) {
                u >>= 1;
                s <<= 1;
            } else if ((v & 1) == 0) {
                v >>= 1;
                r <<= 1;
            } else if (u > v) {
                u = (u - v) >> 1;
                r += s;
                s <<= 1;
            } else {
                v = (v - u) >> 1;
                s += r;
                r <<= 1;
            }
            k++;
            if (r >= p)
                r -= p;
        }
        return std::make_pair(p - r, k);
    }

    static u32 phase2(u32 a) {
        auto [r, k] = phase1(a);
        for (int i = 0; i < k - 32; i++) {
            if ((r & 1) == 0)
                r >>= 1;
            else
                r = (r + mod) >> 1;
        }
        for (int i = 0; i < 32 - k; i++) {
            r <<= 1;
            if (r >= mod)
                r -= mod;
        }
        return r;
    }
};
