#include <iostream>
#include <iomanip>
#include <boost/multiprecision/cpp_dec_float.hpp>

// Use Boost.Multiprecision cpp_dec_float with compile-time specified precision
// Here we choose 1000 decimal digits. Adjust Dibits to desired precision.
constexpr unsigned int Digits = 1000;
typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<Digits>> bigfloat;

// Compute pi using the Chudnovsky algorithm
bigfloat compute_pi(unsigned int terms) {
    bigfloat sum = 0;
    bigfloat M = 1;
    bigfloat L = 13591409;
    bigfloat X = 1;
    const bigfloat C = 426880 * sqrt(bigfloat(10005));

    for (unsigned int k = 0; k < terms; ++k) {
        if (k > 0) {
            // Recurrence relations:
            // M_k = M_{k-1} * ( (12k-6)*(12k-5)*(12k-4) ) / (k*k*k)
            bigfloat kf = k;
            bigfloat numerator = ( (bigfloat(12) * kf - 6) * (bigfloat(12) * kf - 5) * (bigfloat(12) * kf - 4) );
            M = M * numerator / (kf * kf * kf);

            // L_k = L_{k-1} + 545140134
            L += 545140134;

            // X_k = X_{k-1} * (-262537412640768000)
            X *= -262537412640768000ULL;
        }
        // term = M * L / X
        sum += M * L / X;
    }

    // pi = C / sum
    return C / sum;
}

int main(int argc, char* argv[]) {
    unsigned int terms = 5; // default number of terms
    if (argc > 1) {
        terms = std::stoul(argv[1]);
    }

    // Set output precision: Digits
    std::cout << std::setprecision(Digits) << compute_pi(terms) << std::endl;
    return 0;
}
