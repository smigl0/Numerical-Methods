#pragma once
#define LDA 6
#define MAX_RECURSION 150

#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

double dotProd(double* a, double* b){
  double result=0;
  for(size_t i=0;i<LDA;i++){
    result += a[i]*b[i]; 
  } 
  return result;
}

double vectorNorm(double* a){
  double result=0;
  for(size_t i=0;i<LDA;i++){
    result += a[i]*a[i];
  } 
  return std::sqrt(result);
}

void normalize(double* a){
  double norm = vectorNorm(a);
  for(size_t i=0;i<LDA;i++){
    a[i]=a[i]/norm;
  } 
}

void matrixVectorMult(double* v, double* a){
  double result_v[LDA];
  for(int i=0;i<LDA;i++){
    double sum=0;
    for(int j=0;j<LDA;j++){
      sum+=a[i*LDA+j]*v[j];
    }
    result_v[i]= sum;
  }
  for(int i=0;i<LDA;i++){
    v[i] = result_v[i];
  }
}

double rayleighQuotient(double*v, double* a){
  double copy_of_v[LDA];
  for(int i=0;i<LDA;i++){
    copy_of_v[i]=v[i];
  }
  double norm = dotProd(copy_of_v,copy_of_v);
  matrixVectorMult(copy_of_v,a); 
  return dotProd(v,copy_of_v)/norm;
}

std::vector<double> outerProd(double* a, double* b){
  std::vector<double> product = std::vector<double>(LDA*LDA);
  for(size_t i =0;i<LDA;i++){
    for(size_t j =0;j<LDA;j++){
      product[i*LDA+j] = a[i]*b[j];
    }
  }
  return product;
}

std::vector<double> matrixProd(std::vector<double> a, std::vector<double> b){
  // Matrix Aij movement
  std::vector<double> result(LDA*LDA);
  for(int i=0;i<LDA;i++){
    for(int j=0;j<LDA;j++){
      for(int k=0;k<LDA;k++){
        result[i*LDA+j]+= a[i*LDA+k] *b[j*LDA+k];
      }
    }
  }  
  return result;
}

void printMatrix(double* matrix){
  for(int i=0;i<LDA;i++){
    for(int j=0;j<LDA;j++){
      std::cout<<std::fixed<<std::setprecision(2)<<std::setw(7)<<matrix[i*LDA+j]<<" ";
    }
    std::cout<<"\n";
  }

}

