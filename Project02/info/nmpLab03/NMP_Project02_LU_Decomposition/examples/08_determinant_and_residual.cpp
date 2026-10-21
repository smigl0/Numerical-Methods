// E08 | Core: determinant parity and scaled residual; det=-2.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/08_determinant_and_residual.cpp -o bin/08_determinant_and_residual -llapacke -llapack -lblas -lm
// Run: ./bin/08_determinant_and_residual
// LAPACKE installations may require extra -I/-L flags; see README.
#include <lapacke.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    const lapack_int n = 2;
    const std::array<double, 4> A_original = {1,2, 3,4};
    const std::array<double, 2> b_original = {5,11};
    auto lu = A_original;
    auto x = b_original;
    std::array<lapack_int, 2> ipiv{};
    const lapack_int fi = LAPACKE_dgetrf(
        LAPACK_ROW_MAJOR, n, n, lu.data(), n, ipiv.data());
    if (fi != 0) { std::cerr << "E08: dgetrf info=" << fi << '\n'; return 1; }
    int swaps = 0;
    double product = 1;
    for (lapack_int i = 0; i < n; ++i) {
        if (ipiv[i] < i+1 || ipiv[i] > n) return 1;
        if (ipiv[i] != i+1) ++swaps; // Compare against one-based row i+1.
        product *= lu[i*n+i];
    }
    const double det = (swaps % 2 ? -1.0 : 1.0) * product;
    const lapack_int si = LAPACKE_dgetrs(
        LAPACK_ROW_MAJOR, 'N', n, 1, lu.data(), n, ipiv.data(), x.data(), 1);
    if (si != 0) { std::cerr << "E08: dgetrs info=" << si << '\n'; return 1; }
    double rnorm = 0, anorm = 0, xnorm = 0, bnorm = 0;
    for (int i = 0; i < 2; ++i) {
        if (!std::isfinite(x[i])) return 1;
        double r = b_original[i], rowsum = 0;
        for (int j = 0; j < 2; ++j) {
            r -= A_original[i*2+j]*x[j];
            rowsum += std::abs(A_original[i*2+j]);
        }
        if (!std::isfinite(r)) return 1;
        rnorm = std::max(rnorm, std::abs(r));
        anorm = std::max(anorm, rowsum);
        xnorm = std::max(xnorm, std::abs(x[i]));
        bnorm = std::max(bnorm, std::abs(b_original[i]));
    }
    const double denominator = anorm*xnorm+bnorm;
    if (!(denominator > 0) || !std::isfinite(denominator)) return 1;
    const double eta = rnorm/denominator;
    std::cout << std::setprecision(17);
    std::cout << "swaps = " << swaps << "\ndiagonal_product = " << product
              << "\ndet = " << det << "\nx = " << x[0] << ' ' << x[1]
              << "\nresidual_inf = " << rnorm << "\nscaled_residual = " << eta << '\n';
    if (!std::isfinite(det) || !std::isfinite(eta) || swaps != 1 ||
        std::abs(det+2) > 1e-12 || eta > 1e-12 ||
        std::abs(x[0]-1) > 1e-12 || std::abs(x[1]-2) > 1e-12) return 1;
    std::cout << "PASS E08\n";
    return 0;
}
