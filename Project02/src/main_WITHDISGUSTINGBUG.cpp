// std
#include<vector>
#include<iostream>
#include<cmath>
#include<fstream>
#include<string>

// cblas
# if __has_include("cblas.h")
#include<cblas.h>
#endif
// #include<lapacke.h>

const unsigned int MATRIX_LDA = 6;

template<typename T>
class Matrix {
  public:
    size_t lda;
    std::vector<T> vector;

    Matrix() : lda(MATRIX_LDA), vector{std::vector<T>(MATRIX_LDA*MATRIX_LDA,0)} {};
    Matrix(size_t lda) : lda{lda}, vector{std::vector<T>(lda*lda,0)} {};
    Matrix(size_t lda, double value) : lda{lda}, vector{std::vector<T>(lda*lda,value)} {};
    Matrix(double value) : lda(MATRIX_LDA), vector{std::vector<T>(lda*lda,0)} {};

    T& operator[](size_t i){
      return vector[i];
    }

    void printMatrix(){
      for(int i=0;i<this->lda;i++){
        for(int j=0;j<this->lda;j++){
          std::cout<<vector[j+i*lda]<<" ";
        }
        std::cout<<"\n";
      }
    }

};

template <typename T>
class Coords {
  private:
    T coordsxy[2];
  public:
    T& x=coordsxy[0];
    T& y=coordsxy[1];
    Coords() : x{0}, y{0} {};
    Coords(T x) : x{x}, y{0} {};
    Coords(T x, T y) : x{x}, y{y} {};
    Coords(std::initializer_list<T> L) {
      auto it=L.begin(); 
      x = (L.size()>0) ? *it++ : 0; 
      y = (L.size()>1) ? *it++ : 0;
    };

    T& operator[](size_t index){return coordsxy[index];}

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

  Matrix<double> A = Matrix<double>(6,0);
  for(size_t coords_index=0;coords_index<fx.size();coords_index++){
    for(size_t i =0;i<A.lda;i++){
      std::cout<<fx[coords_index];
      A[i + coords_index*A.lda] = std::pow(fx[coords_index].x,i);
    };
  };
  A.printMatrix();
}
