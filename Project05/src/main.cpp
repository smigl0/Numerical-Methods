#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include "matrix_operations.cpp"

int main(){
  std::vector<double> matrix(LDA*LDA);
  std::vector<double> matrix_inverse(LDA*LDA);
  std::vector<double> matrix_reconstructed(LDA*LDA);

  std::vector<std::vector<double>> eigenvectors;
  std::vector<double> eigenvalues;
  std::fstream s("../data/eigenvalues.txt",s.binary | s.trunc | s.in | s.out);

  // Generate matrix
  for(int i=0;i<LDA;i++){
    for(int j=0;j<LDA;j++){
      if(i==j){
        matrix[LDA*i + j] = -2;
      } else if(std::abs(i-j)==1){
        matrix[LDA*i + j] = 1;
      }
      else {
        matrix[LDA*i + j] = 0;
      }
      std::cout<<std::setw(2)<<matrix[LDA*i + j]<<" ";
    }
    std::cout<<"\n";
  }

  // Begin power method
  double v[LDA];
  for(int i=0;i<LDA;i++){
    v[i]=i;
  }

  for(int n=0;n<LDA;n++){
    std::ofstream eigen_stream("../data/eigenvalue"+std::to_string(n+1)+".txt");

    // Power method loop
    int loop_count=0;
    double previous_lambda=0;

    for(int i=0;i<MAX_RECURSION;i++){
      loop_count++;
      matrixVectorMult(v,matrix.data());
      for(auto vector : eigenvectors){
        double scaling_factor = dotProd(v,vector.data());
        for(int i=0;i<LDA;i++){
          v[i] -= scaling_factor * vector[i];
        }
      }
      eigen_stream<<std::setprecision(15)<<rayleighQuotient(v,matrix.data())<<",\n";
      normalize(v);

      double lambda = rayleighQuotient(v,matrix.data());
      if(std::abs(lambda-previous_lambda)<NUMERICAL_ERROR){break;}
      previous_lambda = lambda;
    }

    std::cout<<"λ No."<<n+1<<": "<<rayleighQuotient(v,matrix.data())<<" ("<<loop_count<<" loops)\n";
    s<<std::setprecision(8)<<rayleighQuotient(v,matrix.data())<<",";
    normalize(v);
    eigenvalues.push_back(rayleighQuotient(v,matrix.data()));
    std::vector<double> eigenvector_to_push = std::vector<double>(v,v+LDA);
    normalize(eigenvector_to_push.data());
    eigenvectors.push_back(eigenvector_to_push);

    for(int i=0;i<LDA;i++){
      v[i]=i;
    }
  }
  
  s<<"\n";
  s.close();
  s.open("../data/eigenvectors.txt",s.binary | s.trunc | s.in | s.out);
  for(auto vector : eigenvectors){
    for(auto index : vector){
      s<<index<<",";
    }
    s<<"\n";
  }
  s.close();

  // Recconstruct matrix
  for(int n=0;n<LDA;n++){
    std::vector<double> outer_product_matrix = outerProd(eigenvectors[n].data(),eigenvectors[n].data());
    for(int i=0;i<LDA*LDA;i++){
      matrix_reconstructed[i]+= outer_product_matrix[i]*eigenvalues[n];
      matrix_inverse[i]+= outer_product_matrix[i]*std::pow(eigenvalues[n],-1);
    }
  }

  std::cout<<"-- Inverse \n";
  for(int i=0;i<LDA;i++){
    for(int j=0;j<LDA;j++){
      std::cout<<std::setw(6)<<std::setprecision(2)<<matrix_inverse[j+i*LDA]<<" ";
    }
    std::cout<<"\n";
  }
  
  std::cout<<"-- A^-1 * A\n";
  printMatrix(matrixProd(matrix_reconstructed,matrix_inverse).data());
  return 0;
}
