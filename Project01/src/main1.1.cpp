// std
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
// linked
#include <math.h>

const inline double LDA = 2000;
const inline int ITERATION_COUNT = 30;

void matrix_times_vec(const std::vector<double>* A, std::vector<double>* x){
  std::vector<double> x_new(LDA, 0.0);
  for(int j=0; j<LDA; j++){
    double sum = 0.0;
    for(int i=0; i<LDA; i++){
      sum += (*x)[i] * (*A)[i + j*LDA];
    };
    x_new[j] = sum;
  };
  *x = x_new;
}

double calc_norm(const std::vector<double>* x){
  double sum = 0;
  for(int i=0; i<LDA; i++){
    sum += (*x)[i] * (*x)[i];  
  }
  return sqrt(sum);
}

int main(){
  int GAMMA_VALUES[] = {2, 3};

  for(const double GAMMA_VALUE : GAMMA_VALUES){
    std::cout << GAMMA_VALUE << "\n";
    std::vector<double> x = std::vector<double>(LDA, 0);
    for(int i=0; i<LDA; i++){
      x[i] = 1.0;
    };

    std::ostringstream ss; 
    ss << "../data/gamma" << GAMMA_VALUE << ".txt";
    
    std::ofstream myFile(ss.str(),std::ios::trunc);
    myFile << calc_norm(&x) << "\n";
    std::vector<double> A = std::vector<double>(LDA*LDA, 0);

    for(int j=0; j<LDA; j++){
      for(int i=0; i<LDA; i++){
        A[i + j*LDA] = 1/(pow(abs(i-j),4) + GAMMA_VALUE);
      };
    };

    for(int i=0; i<ITERATION_COUNT; i++){
      matrix_times_vec(&A, &x);
      myFile << calc_norm(&x) << "\n";
    }
    myFile.close();
  }

  return 0;
}
