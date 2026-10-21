// E01 | Core: 2x2 algebra, solution (1,2); residual zero.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/01_small_linear_system.cpp -o bin/01_small_linear_system
// Run: ./bin/01_small_linear_system
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

int main() {
    const std::array<double, 4> A = {2, 1, 1, 1};
    const std::array<double, 2> b = {4, 3};
    // Subtract equation 2 from equation 1, then substitute.
    const double x0 = b[0] - b[1];
    const double x1 = b[1] - x0;
    const std::array<double, 2> x = {x0, x1};
    double residual = 0.0;
    for (int i = 0; i < 2; ++i) {
        const double r = b[i] - A[2*i]*x[0] - A[2*i+1]*x[1];
        if (!std::isfinite(r)) return 1;
        residual = std::max(residual, std::abs(r));
    }
    std::cout << "x = " << x[0] << ' ' << x[1] << '\n';
    std::cout << "residual_inf = " << residual << '\n';
    if (!std::isfinite(x0) || !std::isfinite(x1) ||
        std::abs(x0-1) > 1e-12 || std::abs(x1-2) > 1e-12 ||
        residual > 1e-12) {
        std::cerr << "E01: solution check failed\n";
        return 1;
    }
    std::cout << "PASS E01\n";
    return 0;
}
