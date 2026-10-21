// E02 | Core: row-major 2x3 layout [1,2,3,4,5,6], lda=3; optional padding.
// Build from the extracted package root after: mkdir -p bin data plots
// g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 examples/02_row_major_storage.cpp -o bin/02_row_major_storage
// Run: ./bin/02_row_major_storage
#include <array>
#include <iostream>

int main() {
    const int rows = 2, cols = 3, lda = 3;
    std::array<double, 6> a{};
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            a[i*lda+j] = i*cols+j+1;
        }
    }
    std::cout << "row_major =";
    for (double value : a) std::cout << ' ' << value;
    std::cout << '\n';
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const int offset = i*lda+j;
            std::cout << "A(" << i << ',' << j << ")=" << a[offset]
                      << " offset=" << offset << '\n';
            // These small integers are exactly representable.
            if (a.at(offset) != i*cols+j+1) return 1;
        }
    }
    // The leading dimension is a physical stride, not the row count.
    const int padded_lda = 5;
    std::array<double, 10> padded{};
    padded.fill(-99);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            padded[i*padded_lda+j] = a[i*lda+j];
    std::cout << "padded =";
    for (double value : padded) std::cout << ' ' << value;
    std::cout << '\n';
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < padded_lda; ++j)
            if (padded.at(i*padded_lda+j) !=
                (j < cols ? i*cols+j+1 : -99)) return 1;
    std::cout << "PASS E02\n";
    return 0;
}
