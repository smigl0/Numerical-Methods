// E04: One explicit power iteration for B=[[2,1],[1,2]].
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
int main(){
 const std::array<double,2>x{1,0},y{2*x[0]+x[1],x[0]+2*x[1]};
 const double length=std::hypot(y[0],y[1]);
 const std::array<double,2>next{y[0]/length,y[1]/length};
 std::cout<<std::setprecision(15)<<"y0="<<y[0]<<" y1="<<y[1]<<" norm_y="<<length<<" next0="<<next[0]<<" next1="<<next[1]<<"\n";
 if(std::abs(length-std::sqrt(5.))>1e-14||std::abs(next[0]-2/std::sqrt(5.))>1e-14||std::abs(next[1]-1/std::sqrt(5.))>1e-14)return 1;
 std::cout<<"PASS E04\n";
}
