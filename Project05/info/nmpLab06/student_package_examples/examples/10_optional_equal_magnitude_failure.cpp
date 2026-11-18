// OPTIONAL E10.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
int main(){
 const double s=1/std::sqrt(2.);std::array<double,2> x{s,s};
 for(int k=0;k<8;k++){
  std::array<double,2> y{x[0],-x[1]};
  double lam=x[0]*y[0]+x[1]*y[1];double r=std::hypot(y[0]-lam*x[0],y[1]-lam*x[1]);
  if(k<3)std::cout<<std::setprecision(15)<<"iteration="<<k<<" rayleigh="<<lam<<" residual="<<r<<" x1="<<x[1]<<"\n";
  if(std::abs(lam)>1e-13||std::abs(r-1)>1e-13)return 1;
  x=y;
 }
 std::cout<<"NO RESIDUAL CONVERGENCE (expected)\nPASS E10\n";
}
