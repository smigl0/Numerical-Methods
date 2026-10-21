// E03 | Core: unpivoted 3x3 teaching fixture ONLY; multipliers 2,4,3; x=(1,1,1).
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/03_gaussian_elimination.cpp -o bin/03_gaussian_elimination
// Run: ./bin/03_gaussian_elimination
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        constexpr int n = 3;
        const std::array<double, 9> A_original = {2,1,1, 4,3,3, 8,7,9};
        const std::array<double, 3> b_original = {4,10,24};
        auto U = A_original;
        auto rhs = b_original;
        // Unpivoted elimination is safe for THIS fixed fixture only.
        // Real general systems need partial pivoting (E05/E06).
        for (int k = 0; k < n-1; ++k) {
            if (U[k*n+k] == 0) throw std::runtime_error("Zero pivot");
            for (int i = k+1; i < n; ++i) {
                const double m = U[i*n+k] / U[k*n+k];
                std::cout << "m(" << i << ',' << k << ")=" << m << '\n';
                for (int j = k+1; j < n; ++j)
                    U[i*n+j] -= m * U[k*n+j];
                U[i*n+k] = 0; // The eliminated location is explicitly zero.
                rhs[i] -= m * rhs[k];
            }
        }
        std::array<double, 3> x{};
        for (int i = n-1; i >= 0; --i) {
            double sum = rhs[i];
            for (int j = i+1; j < n; ++j) sum -= U[i*n+j]*x[j];
            if (U[i*n+i] == 0) throw std::runtime_error("Zero pivot");
            x[i] = sum / U[i*n+i];
        }
        std::cout << "U =";
        for (double v : U) std::cout << ' ' << v;
        std::cout << "\nrhs =";
        for (double v : rhs) std::cout << ' ' << v;
        std::cout << "\nx =";
        for (double v : x) std::cout << ' ' << v;
        std::cout << '\n';
        const std::array<double, 9> expected_U = {2,1,1, 0,1,1, 0,0,2};
        const std::array<double, 3> expected_rhs = {4,2,2};
        for (int k = 0; k < n*n; ++k)
            if (!std::isfinite(U[k]) || std::abs(U[k]-expected_U[k]) > 1e-12)
                throw std::runtime_error("Upper-factor check failed");
        double residual = 0;
        for (int i = 0; i < n; ++i) {
            if (!std::isfinite(rhs[i]) || !std::isfinite(x[i]) ||
                std::abs(rhs[i]-expected_rhs[i]) > 1e-12 ||
                std::abs(x[i]-1) > 1e-12)
                throw std::runtime_error("Solution check failed");
            double r = b_original[i];
            for (int j = 0; j < n; ++j) r -= A_original[i*n+j]*x[j];
            residual = std::max(residual, std::abs(r));
        }
        std::cout << "residual_inf = " << residual << '\n';
        if (residual > 1e-12) throw std::runtime_error("Residual check failed");
        std::cout << "PASS E03\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "E03: " << e.what() << '\n';
        return 1;
    }
}
