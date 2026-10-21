// std
#include<vector>
#include<iostream>
#include<cmath>
#include<iomanip>
#include<fstream>
#include<string>

// cblas
#include<openblas/cblas.h>
#include<openblas/lapacke.h>

const unsigned int MATRIX_LDA = 6;

template<typename T>
class Matrix {
  public:
    size_t lda;
    std::vector<T> vector;

    Matrix() : lda(MATRIX_LDA), vector{std::vector<T>(MATRIX_LDA*MATRIX_LDA,0)} {};
    Matrix(size_t lda) : lda{lda}, vector{std::vector<T>(lda*lda,0)} {};
    Matrix(size_t lda, T value) : lda{lda}, vector{std::vector<T>(lda*lda,value)} {};

    T& operator[](size_t i){
      return vector[i];
    }

    void printMatrix(){
      for(int i=0;i<this->lda;i++){
        for(int j=0;j<this->lda;j++){
          std::cout<<std::setw(6)<<std::setprecision(2)<<std::fixed<<vector[j+i*lda]<<" ";
        }
        std::cout<<"\n";
      }
    }

};

template <typename T>
class Coords {
  public:
    T x;
    T y;
    Coords() : x{0}, y{0} {};
    Coords(T x) : x{x}, y{0} {};
    Coords(T x, T y) : x{x}, y{y} {};
    Coords(std::initializer_list<T> L) {
      auto it=L.begin(); 
      x = (L.size()>0) ? *it++ : 0; 
      y = (L.size()>1) ? *it++ : 0;
    };

    T& operator[](size_t index){return (index==0) ? x : y;}

    friend std::ostream& operator<<(std::ostream& os, const Coords coords){
      os << "{" << coords.x<<","<<coords.y<<"}";
      return os;
    }

};


int main(){
  std::vector<Coords<double>> fx = {
    {0,0},{1,1},{2,0},{3,-1}
  };

  std::vector<Coords<double>> dfx = {
    {0,1},{3,0}
  };

  // GENERATE THE MATRIX A

  Matrix<double> A = Matrix<double>(6,0);
  for(size_t coords_index=0;coords_index<fx.size();coords_index++){
    for(size_t i =0;i<A.lda;i++){
      A[i + coords_index*A.lda] = std::pow(fx[coords_index].x,i);
    };
  };

  for(size_t coords_index=fx.size();coords_index<fx.size()+dfx.size();coords_index++){
    for(size_t i =0; i<A.lda; i++){
      if(i==0){
        A[i + coords_index*A.lda] =0;
      } else if(i==1){
        A[i + coords_index*A.lda] =1;
      } else {
        A[i + coords_index*A.lda] = i*std::pow(dfx[coords_index-fx.size()].x,i-1);
      }
    };
  };

  A.printMatrix();
  std::cout<<"--- FACTORIZING ---\n";
  // LU FACTORIZE
  std::vector<int> ipiv(A.lda);
  LAPACKE_dgetrf(
      LAPACK_ROW_MAJOR,
      A.lda,
      A.lda,
      A.vector.data(),
      A.lda,
      ipiv.data());
  std::cout<<"row swap vector:\n";
  for(int row_swap : ipiv){
    std::cout<<row_swap<<" ";
  };
  std::cout<<std::endl;
  std::cout<<"--- FACTORIZED IN PLACE LU MATRIX ---\n";
  A.printMatrix();

  // CALCULATE THE DETERMINANT 
  double det = 1.0;
  int swaps = 0;
  for (size_t i = 0; i < MATRIX_LDA; i++) {
    det *= A[i * MATRIX_LDA + i];
    if (ipiv[i] != i+1) {
      swaps++;
    }
  }
  if (swaps % 2 != 0) {
    det = -det;
  }

  std::cout<<"Determinant of U: "<<det<<std::endl;

  // CONSTRUCT THE Y VECTOR
  std::vector<double> y;
  for(Coords coords : fx){
    y.push_back(coords.y); 
  }
  for(Coords coords : dfx){
    y.push_back(coords.y); 
  }
  
  LAPACKE_dgetrs(
      LAPACK_ROW_MAJOR,
      'N',
      A.lda,
      1,
      A.vector.data(),
      A.lda,
      ipiv.data(),
      y.data(),
      1
    );
  std::cout<<"--- SOLVED COEFFICIENTS ---\n";
  for(double y_val : y){
    std::cout<<std::setw(10)<<std::setprecision(6)<<y_val<<" ";
  };
  std::cout<<"\n";
  return 0;
}
