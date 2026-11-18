// E02: Assemble a small finite-difference matrix; print all entries.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
int main(){
 std::array<std::array<double,3>,3> A{};
 for(int i=0;i<3;i++)for(int j=0;j<3;j++) A[i][j]=(i==j?-2.0:(std::abs(i-j)==1?1.0:0.0));
 std::array<double,3> v{1,2,3},expected{0,0,-4},y{};
 std::cout<<"matrix:\n";
 for(int i=0;i<3;i++) {for(int j=0;j<3;j++){std::cout<<std::setw(5)<<A[i][j];y[i]+=A[i][j]*v[j];}std::cout<<'\n';}
 for(int i=0;i<3;i++){std::cout<<"y"<<i<<"="<<y[i]<<"\n";if(std::abs(y[i]-expected[i])>1e-12)return 1;}
 std::cout<<"PASS E02\n";
}
