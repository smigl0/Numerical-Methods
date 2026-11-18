// E06: One Rayleigh quotient and its independently calculated residual.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
int main(){
 const std::array<double,2> x{2,1}; const std::array<double,2> Bx{5,4};
 const double num=x[0]*Bx[0]+x[1]*Bx[1],den=x[0]*x[0]+x[1]*x[1],lambda=num/den;
 const double s=std::sqrt(den),r=std::hypot((Bx[0]-lambda*x[0])/s,(Bx[1]-lambda*x[1])/s);
 std::cout<<std::setprecision(15)<<"numerator="<<num<<" denominator="<<den<<" rayleigh="<<lambda<<" residual="<<r<<"\n";
 if(std::abs(num-14)>1e-13||std::abs(den-5)>1e-13||std::abs(lambda-2.8)>1e-13||std::abs(r-0.6)>1e-13)return 1;
 std::cout<<"PASS E06\n";
}
