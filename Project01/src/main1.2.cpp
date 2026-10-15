#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

const int LDA = 2000;
const int ITERATION_COUNT = 30;

std::vector<double> squareMatrix(const std::vector<double>& A, int LDA) {
  std::vector<double> C(LDA * LDA, 0.0);

  for (int i = 0; i < LDA; i++) {
    for (int k = 0; k < LDA; k++) {
      double s = 0.0;
      for (int j = 0; j < LDA; j++) {
        s += A[i * LDA + j] * A[j * LDA + k];
      }
      C[i * LDA + k] = s;
    }
  }

  return C;
}

int main() {
  int GAMMA_VALUES[] = {2, 3};

  for (const double GAMMA_VALUE : GAMMA_VALUES) {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<double> A(LDA * LDA, 0.0);

    for(int j=0;j<LDA;j++){
      for(int i=0;i<LDA;i++){
        A[i + j*LDA] = 1/(pow(abs(i-j),4) + GAMMA_VALUE);
      };
    };

    std::vector<double> C = squareMatrix(A, LDA);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    // Fixed: use .count() and divide by 1000.0 to get accurate seconds
    std::cout << "Successfully computed C = A * A for GAMMA = " << GAMMA_VALUE 
      << "\n In: " << (elapsed.count() / 1000.0) << "s\n";
  }

  return 0;
}
