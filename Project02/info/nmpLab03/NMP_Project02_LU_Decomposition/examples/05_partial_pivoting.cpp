// E05 | Core: 2x2 zero-pivot example; one row swap; det=-2, x=(1,2).
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/05_partial_pivoting.cpp -o bin/05_partial_pivoting
// Run: ./bin/05_partial_pivoting
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

int main() {
    const std::array<double, 4> A_original = {0,1, 2,3};
    const std::array<double, 2> b_original = {2,8};
    auto U = A_original;
    auto rhs = b_original;
    // Find the largest absolute entry in the first active column.
    const int pivot_row = (std::abs(U[2]) > std::abs(U[0])) ? 1 : 0;
    int swaps = 0;
    if (pivot_row != 0) {
        for (int j = 0; j < 2; ++j) std::swap(U[j], U[2+j]);
        std::swap(rhs[0], rhs[1]); // Apply the SAME operation to b.
        ++swaps;
    }
    if (U[0] == 0) return 1;
    const double m = U[2] / U[0];
    U[3] -= m * U[1];
    U[2] = 0;
    rhs[1] -= m * rhs[0];
    if (U[3] == 0) return 1;
    const double x1 = rhs[1] / U[3];
    const double x0 = (rhs[0]-U[1]*x1) / U[0];
    const double det = (swaps % 2 ? -1.0 : 1.0) * U[0]*U[3];
    const double r0 = b_original[0]-A_original[0]*x0-A_original[1]*x1;
    const double r1 = b_original[1]-A_original[2]*x0-A_original[3]*x1;
    const double residual = std::max(std::abs(r0),std::abs(r1));
    std::cout << "swaps = " << swaps << "\nx = " << x0 << ' ' << x1
              << "\ndet = " << det << "\nresidual_inf = " << residual << '\n';
    if (!std::isfinite(x0) || !std::isfinite(x1) || !std::isfinite(det) ||
        !std::isfinite(residual) || swaps != 1 || std::abs(det+2) > 1e-12 ||
        std::abs(x0-1) > 1e-12 || std::abs(x1-2) > 1e-12 ||
        residual > 1e-12) return 1;
    std::cout << "PASS E05\n";
    return 0;
}
