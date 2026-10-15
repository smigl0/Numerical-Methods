// std
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
// linked
#include <math.h>

const inline double LDA = 2000;
const inline int ITERATION_COUNT = 30;

void matrix_times_vec(std::vector<double> A, std::vector<double>* x){
  for(int j=0;j<LDA;j++){
    for(int i=0;i<LDA;i++){
      (*x)[j] += (*x)[i]*A[i+j*LDA];
    };
  };
}

double calc_norm(std::vector<double> x ){
  double sum=0;
  for(int i=0; i<LDA;i++){
    sum += x[i]*x[i]; 
  }
  return sqrt(sum);
}

int main(){
  int GAMMA_VALUES[] = {2,3};

  std::ofstream myFile;
  myFile.open("../data/task1.1.txt");
  for(const double GAMMA_VALUE : GAMMA_VALUES){

    std::vector<double> x = std::vector<double>(LDA,0);
    for(int i=0;i<LDA;i++){
      x[i] = (double)i;
    };
    myFile<<"0:"<<calc_norm(x)<<"\n";

    std::vector<double> A = std::vector<double>(LDA*LDA,0);

    for(int j=0;j<LDA;j++){
      for(int i=0;i<LDA;i++){
        A[i + j*LDA] = 1/(pow(abs(i-j),4) + GAMMA_VALUE);
      };
    };

    myFile<<"--- GAMMA VALUE= "<<GAMMA_VALUE<<"\n";
    for(int i=0;i<ITERATION_COUNT;i++){
      matrix_times_vec(A,&x);
      myFile<<i+1<<":"<<calc_norm(x)<<"\n";
    }
  }
  myFile.close();

  return 0;
}
