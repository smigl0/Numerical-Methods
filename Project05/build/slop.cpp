#include <vector>
#include <iomanip>
#include <iostream>
#include <cmath>

#define LDA 6
#define MAX_RECURSION 150

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

int main(){
  double matrix [LDA*LDA];
  std::vector<std::vector<double>> eigenvectors;

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
  double v[] = {1,1,1,1,1,1};

  for(int n=0;n<LDA;n++){
    // Power method loop (UNTOUCHED)
    for(int i=0;i<MAX_RECURSION;i++){
      matrixVectorMult(v,matrix);
      normalize(v);
    }
    
    // --- SCRUB DRIFT HERE (Outside the inner loop, using your notes formula) ---
    for(auto& vector : eigenvectors){
      double scaling_factor = dotProd(v, vector.data());
      for(int k=0; k<LDA; k++){
        v[k] -= scaling_factor * vector[k];
      }
    }
    normalize(v);
    // --------------------------------------------------------------------------

    // Print loop
    std::cout<<"λ No."<<n<<": "<<rayleighQuotient(v,matrix)<<"\n";
    
    eigenvectors.push_back(std::vector<double>(v,v+LDA));

    double new_guess[] = {1,1,1,1,1,1};
    for(auto vector : eigenvectors){
      double scaling_factor = dotProd(new_guess,vector.data());
      for(int i=0;i<LDA;i++){
        new_guess[i] -= scaling_factor * vector[i];
      }
    }
    normalize(new_guess);
    for(int i=0; i<LDA; i++) v[i] = new_guess[i];
  }
  return 0;
}
