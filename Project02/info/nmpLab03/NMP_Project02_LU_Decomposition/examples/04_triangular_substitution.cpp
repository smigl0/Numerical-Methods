// E04 | Core: forward/back substitution; y=(4,2,2), x=(1,1,1).
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/04_triangular_substitution.cpp -o bin/04_triangular_substitution
// Run: ./bin/04_triangular_substitution
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

int main() {
    constexpr int n = 3;
    const std::array<double, 9> L = {1,0,0, 2,1,0, 4,3,1};
    const std::array<double, 9> U = {2,1,1, 0,1,1, 0,0,2};
    const std::array<double, 3> b = {4,10,24};
    std::array<double, 3> y{}, x{};
    // Forward substitution: earlier y values are already available.
    for (int i = 0; i < n; ++i) {
        double sum = b[i];
        for (int j = 0; j < i; ++j) sum -= L[i*n+j]*y[j];
        if (L[i*n+i] == 0) return 1;
        y[i] = sum / L[i*n+i];
    }
    // Use a signed index: an unsigned i would underflow below zero.
    for (int i = n-1; i >= 0; --i) {
        double sum = y[i];
        for (int j = i+1; j < n; ++j) sum -= U[i*n+j]*x[j];
        if (U[i*n+i] == 0) return 1;
        x[i] = sum / U[i*n+i];
    }
    std::cout << "y =";
    for (double v : y) std::cout << ' ' << v;
    std::cout << "\nx =";
    for (double v : x) std::cout << ' ' << v;
    std::cout << '\n';
    const std::array<double, 3> expected_y = {4,2,2};
    double lower_residual = 0, upper_residual = 0;
    for (int i = 0; i < n; ++i) {
        if (!std::isfinite(x[i]) || !std::isfinite(y[i]) ||
            std::abs(x[i]-1) > 1e-12 ||
            std::abs(y[i]-expected_y[i]) > 1e-12) return 1;
        double rL = b[i], rU = y[i];
        for (int j = 0; j < n; ++j) {
            rL -= L[i*n+j]*y[j];
            rU -= U[i*n+j]*x[j];
        }
        lower_residual = std::max(lower_residual, std::abs(rL));
        upper_residual = std::max(upper_residual, std::abs(rU));
    }
    std::cout << "lower_residual = " << lower_residual << '\n';
    std::cout << "upper_residual = " << upper_residual << '\n';
    if (lower_residual > 1e-12 || upper_residual > 1e-12) return 1;
    std::cout << "PASS E04\n";
    return 0;
}
