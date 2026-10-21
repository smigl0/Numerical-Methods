// E09 | Core: toy q(x)=1+x-x^2; five CSV rows, not the official polynomial.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/09_toy_polynomial_output.cpp -o bin/09_toy_polynomial_output
// Run: ./bin/09_toy_polynomial_output data/toy_run01.csv
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) throw std::runtime_error("Use: program NEW_OUTPUT.csv");
        const std::filesystem::path output = argv[1];
        if (std::filesystem::exists(output))
            throw std::runtime_error("Output exists; choose a NEW filename");
        // Deliberately do not create unknown directory trees.
        // Run mkdir -p data first, or supply an existing parent directory.
        const std::array<double, 5> xs = {0,0.5,1,1.5,2};
        const std::array<double, 5> expected = {1,1.25,1,0.25,-1};
        std::array<double, 5> ys{};
        for (int i = 0; i < 5; ++i) {
            const double x = xs[i];
            ys[i] = 1 + x*(1-x); // q(x)=1+x-x^2, evaluated in Horner form.
            if (!std::isfinite(ys[i]) || std::abs(ys[i]-expected[i]) > 1e-12)
                throw std::runtime_error("Toy polynomial check failed");
        }
        std::ofstream csv(output);
        if (!csv) throw std::runtime_error("Cannot open output; check its parent");
        csv << "x,y\n" << std::setprecision(std::numeric_limits<double>::max_digits10);
        for (int i = 0; i < 5; ++i) csv << xs[i] << ',' << ys[i] << '\n';
        csv.close();
        if (!csv) throw std::runtime_error("Output write/close failed");
        std::cout << "rows = 5\nSaved: " << output.string() << "\nPASS E09\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "E09: " << e.what() << '\n';
        return 1;
    }
}
