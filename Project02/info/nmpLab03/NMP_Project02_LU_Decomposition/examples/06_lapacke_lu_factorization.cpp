// E06 | Core: real LAPACKE factorization; A=[1,2;3,4], PA=LU.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/06_lapacke_lu_factorization.cpp -o bin/06_lapacke_lu_factorization -llapacke -llapack -lblas -lm
// Run: ./bin/06_lapacke_lu_factorization
// LAPACKE installations may require extra -I/-L flags; see README.
#include <lapacke.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    const lapack_int n = 2, lda = 2;
    const std::array<double, 4> A_original = {1,2, 3,4};
    auto lu = A_original;
    std::array<lapack_int, 2> ipiv{};
    const lapack_int info = LAPACKE_dgetrf(
        LAPACK_ROW_MAJOR, n, n, lu.data(), lda, ipiv.data());
    std::cout << "info = " << info << '\n';
    if (info != 0) {
        std::cerr << "E06: factorization failed; inspect info above.\n";
        return 1;
    }
    std::array<double, 4> L{}, U{};
    auto PA = A_original;
    for (lapack_int i = 0; i < n; ++i) {
        const lapack_int p = ipiv[i]-1; // LAPACK values are ONE-based.
        if (p < i || p >= n) return 1;
        for (lapack_int j = 0; j < n; ++j)
            std::swap(PA[i*n+j], PA[p*n+j]);
        for (lapack_int j = 0; j < n; ++j) {
            L[i*n+j] = (i > j) ? lu[i*n+j] : (i == j ? 1.0 : 0.0);
            U[i*n+j] = (i <= j) ? lu[i*n+j] : 0.0;
        }
    }
    double factor_residual = 0;
    for (lapack_int i = 0; i < n; ++i) {
        for (lapack_int j = 0; j < n; ++j) {
            double product = 0;
            for (lapack_int k = 0; k < n; ++k) product += L[i*n+k]*U[k*n+j];
            if (!std::isfinite(product) || !std::isfinite(lu[i*n+j])) return 1;
            factor_residual = std::max(factor_residual, std::abs(PA[i*n+j]-product));
        }
    }
    std::cout << std::setprecision(17) << "lu =";
    for (double v : lu) std::cout << ' ' << v;
    std::cout << "\nipiv = " << ipiv[0] << ' ' << ipiv[1] << "\nL =";
    for (double v : L) std::cout << ' ' << v;
    std::cout << "\nU =";
    for (double v : U) std::cout << ' ' << v;
    std::cout << "\nPA =";
    for (double v : PA) std::cout << ' ' << v;
    std::cout << "\nfactor_residual = " << factor_residual << '\n';
    if (factor_residual > 1e-12 || ipiv[0] != 2 || ipiv[1] != 2) return 1;
    std::cout << "PASS E06\n";
    return 0;
}
