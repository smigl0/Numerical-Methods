// E10 | OPTIONAL: conditioning; fixed RHS perturbation.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/10_conditioning_experiment.cpp -o bin/10_conditioning_experiment -llapacke -llapack -lblas -lm
// Run: ./bin/10_conditioning_experiment
// LAPACKE installations may require extra -I/-L flags; see README.
#include <lapacke.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    // OPTIONAL. b=(1,1) gives exact baseline x=(1,0) for epsilon>0.
    // Perturb b to (1,1+delta): x=(1-delta/epsilon,delta/epsilon).
    // Powers of two expose rounding without decimal conversion ambiguity.
    const double delta = std::ldexp(1.0,-40);
    const std::array<int, 6> powers = {-4,-16,-28,-40,-52,-54};
    std::cout << std::setprecision(17);
    std::cout << "delta = " << delta << '\n';
    std::cout << "double_epsilon = " << std::numeric_limits<double>::epsilon() << '\n';
    int solved = 0, singular = 0;
    for (int k = 0; k < 7; ++k) {
        const double requested = k < 6 ? std::ldexp(1.0,powers[k]) : 0.0;
        const std::array<double, 4> A_original = {1,1, 1,1+requested};
        const double stored = A_original[3]-1;
        auto lu = A_original;
        std::array<lapack_int, 2> ipiv{};
        const lapack_int info = LAPACKE_dgetrf(
            LAPACK_ROW_MAJOR, 2, 2, lu.data(), 2, ipiv.data());
        if (info < 0) { std::cerr << "E10: dgetrf info=" << info << '\n'; return 1; }
        if (info > 0) {
            if (stored != 0) return 1;
            ++singular;
            std::cout << "SINGULAR requested=" << requested << " stored=" << stored
                      << " info=" << info << " solve_skipped\n";
            continue;
        }
        if (!(stored > 0)) return 1;
        // First solve: exact baseline b. Second solve: perturbed b.
        std::array<double, 2> baseline = {1,1};
        const std::array<double, 2> b_original = {1,1+delta};
        auto x = b_original;
        const lapack_int bi = LAPACKE_dgetrs(
            LAPACK_ROW_MAJOR, 'N', 2, 1, lu.data(), 2, ipiv.data(), baseline.data(), 1);
        const lapack_int si = LAPACKE_dgetrs(
            LAPACK_ROW_MAJOR, 'N', 2, 1, lu.data(), 2, ipiv.data(), x.data(), 1);
        if (bi != 0 || si != 0) return 1;
        const double exact_x1 = (b_original[1]-b_original[0])/stored;
        const std::array<double, 2> expected = {1-exact_x1, exact_x1};
        double shift = 0, residual = 0;
        for (int i = 0; i < 2; ++i) {
            const double base_expected = i == 0 ? 1.0 : 0.0;
            if (!std::isfinite(baseline[i]) || !std::isfinite(x[i]) ||
                std::abs(baseline[i]-base_expected) > 1e-12 ||
                std::abs(x[i]-expected[i]) > 1e-10*(1+std::abs(expected[i]))) return 1;
            shift = std::max(shift,std::abs(x[i]-baseline[i]));
            double r = b_original[i];
            for (int j = 0; j < 2; ++j) r -= A_original[2*i+j]*x[j];
            if (!std::isfinite(r)) return 1;
            residual = std::max(residual,std::abs(r));
        }
        double det = lu[0]*lu[3];
        for (lapack_int i = 0; i < 2; ++i) if (ipiv[i] != i+1) det = -det;
        if (!std::isfinite(det) || std::abs(det-stored) > 1e-12*stored ||
            residual > 1e-10*(1+std::max(std::abs(x[0]),std::abs(x[1])))) return 1;
        ++solved;
        std::cout << "CASE requested=" << requested << " stored=" << stored
                  << " det=" << det << " x0=" << x[0] << " x1=" << x[1]
                  << " shift=" << shift << " residual=" << residual << '\n';
    }
    // On binary64: five solves; 2^-54 rounds away in 1+epsilon, plus epsilon=0.
    // Do not require identical case counts on a non-binary64 implementation.
    if (solved == 0 || singular == 0) return 1;
    std::cout << "solved = " << solved << "\nsingular = " << singular << "\nPASS E10\n";
    return 0;
}
