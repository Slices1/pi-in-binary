#include <iostream>
#include <vector>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

// Compute integer square root of n: floor(sqrt(n))
cpp_int integer_sqrt(const cpp_int& n) {
    cpp_int low = 0;
    cpp_int high = cpp_int(1) << ((boost::multiprecision::msb(n) / 2) + 1);
    while (low < high) {
        cpp_int mid = (low + high + 1) >> 1;
        if (mid * mid <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

// Binary-splitting for Chudnovsky terms over [a, b)
// Returns tuple (P, Q, T)
std::tuple<cpp_int, cpp_int, cpp_int> bs(unsigned long a, unsigned long b) {
    if (b - a == 1) {
        // base case k = a
        cpp_int P = 1;
        // P = (6a)! / ((3a)! * (a!)^3)
        for (unsigned long i = 1; i <= 6*a; ++i) P *= i;
        cpp_int denom = 1;
        for (unsigned long i = 1; i <= 3*a; ++i) denom *= i;
        for (unsigned long i = 1; i <= a;   ++i) denom *= i;
        for (unsigned long i = 1; i <= a;   ++i) denom *= i;
        for (unsigned long i = 1; i <= a;   ++i) denom *= i;
        P /= denom;
        if (a & 1) P = -P;
        cpp_int Q = 1;
        // Q = 640320^(3a)
        for (unsigned long i = 0; i < 3*a; ++i) Q *= 640320;
        cpp_int T = P * (13591409 + 545140134 * cpp_int(a));
        return {P, Q, T};
    }
    unsigned long m = (a + b) >> 1;
    auto [P1, Q1, T1] = bs(a, m);
    auto [P2, Q2, T2] = bs(m, b);
    cpp_int P = P1 * P2;
    cpp_int Q = Q1 * Q2;
    cpp_int T = T1 * Q2 + P1 * T2;
    return {P, Q, T};
}

int main(int argc, char* argv[]) {
    unsigned long terms = 5;    // series terms
    unsigned int bits   = 100;  // binary bits of precision
    if (argc > 1) terms = std::stoul(argv[1]);
    if (argc > 2) bits  = std::stoul(argv[2]);

    // 1) Binary-split to get P,Q,T over [0, terms)
    cpp_int P, Q, T;
    std::tie(P, Q, T) = bs(0, terms);

    // 2) Compute scaled constant: C = 426880 * floor(sqrt(10005) * 2^bits)
    cpp_int scale = cpp_int(1) << (2 * bits);
    cpp_int sqrt_n = integer_sqrt(cpp_int(10005) * scale);
    cpp_int C = 426880 * sqrt_n;

    // 3) pi_scaled = (Q * C) / T  yields floor(pi * 2^bits)
    cpp_int pi_scaled = (Q * C) / T;

    // 4) Extract integer and fractional parts
    cpp_int int_part  = pi_scaled >> bits;
    cpp_int frac_mask = (cpp_int(1) << bits) - 1;
    cpp_int frac_part = pi_scaled & frac_mask;

    // 5) Convert to binary strings
    auto to_bin = [&](const cpp_int& x) {
        if (x == 0) return std::string("0");
        cpp_int v = x;
        std::string s;
        while (v > 0) {
            s.push_back((v & 1) ? '1' : '0');
            v >>= 1;
        }
        std::reverse(s.begin(), s.end());
        return s;
    };

    std::string int_str  = to_bin(int_part);
    std::string frac_str = to_bin(frac_part);

    // Pad fractional to 'bits' length
    if (frac_str.length() < bits)
        frac_str = std::string(bits - frac_str.length(), '0') + frac_str;

    std::cout << int_str << "." << frac_str << std::endl;
    return 0;
}
