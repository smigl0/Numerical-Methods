#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// ---
#include <cblas.h>

const int LDA = 2000;
const int ITERATION_COUNT = 30;

std::vector<double> squareMatrix(const std::vector<double>& A, int LDA) {
  std::vector<double> C(LDA * LDA, 0.0);

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
              LDA, LDA, LDA, 1.0, A.data(), LDA, A.data(), LDA, 0.0, C.data(), LDA);

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
    std::cout << "Successfully computed C = A * A for GAMMA = " << GAMMA_VALUE << "\n In: "<<(elapsed.count()/1000)<<"s\n";
  }

  return 0;
}
