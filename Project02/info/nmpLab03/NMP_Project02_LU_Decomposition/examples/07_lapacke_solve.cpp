// E07 | Core: real LAPACKE solve; A=[1,2;3,4], b=(5,11), x=(1,2).
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/07_lapacke_solve.cpp -o bin/07_lapacke_solve -llapacke -llapack -lblas -lm
// Run: ./bin/07_lapacke_solve
// LAPACKE installations may require extra -I/-L flags; see README.
#include <lapacke.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    const lapack_int n = 2, nrhs = 1, lda = 2, ldb = 1;
    const std::array<double, 4> A_original = {1,2, 3,4};
    const std::array<double, 2> b_original = {5,11};
    auto lu = A_original;
    auto x = b_original; // x initially contains b; dgetrs overwrites it.
    std::array<lapack_int, 2> ipiv{};
    const lapack_int factor_info = LAPACKE_dgetrf(
        LAPACK_ROW_MAJOR, n, n, lu.data(), lda, ipiv.data());
    if (factor_info != 0) {
        std::cerr << "E07: dgetrf info=" << factor_info << '\n';
        return 1; // Do not solve with failed or singular factors.
    }
    const lapack_int solve_info = LAPACKE_dgetrs(
        LAPACK_ROW_MAJOR, 'N', n, nrhs, lu.data(), lda,
        ipiv.data(), x.data(), ldb);
    if (solve_info != 0) {
        std::cerr << "E07: dgetrs info=" << solve_info << '\n';
        return 1;
    }
    double residual = 0;
    for (int i = 0; i < 2; ++i) {
        if (!std::isfinite(x[i]) || std::abs(x[i]-(i+1)) > 1e-12) return 1;
        double r = b_original[i];
        for (int j = 0; j < 2; ++j) r -= A_original[2*i+j]*x[j];
        if (!std::isfinite(r)) return 1;
        residual = std::max(residual, std::abs(r));
    }
    std::cout << std::setprecision(17);
    std::cout << "factor_info = " << factor_info << "\nsolve_info = " << solve_info
              << "\nx = " << x[0] << ' ' << x[1]
              << "\nresidual_inf = " << residual << '\n';
    if (residual > 1e-12) return 1;
    std::cout << "PASS E07\n";
    return 0;
}
