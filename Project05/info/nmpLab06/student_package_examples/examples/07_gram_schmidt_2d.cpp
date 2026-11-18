// E07: Projection and Gram-Schmidt with an exact two-vector result.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
using V=std::array<double,2>;
double dot(V x,V y){return x[0]*y[0]+x[1]*y[1];}
int main(){
 const double s=1/std::sqrt(2.);V q{s,s},y{2,0};double projection=dot(q,y);
 V perpendicular{y[0]-projection*q[0],y[1]-projection*q[1]};double length=std::sqrt(dot(perpendicular,perpendicular));
 if(!(length>0))return 1;
 V q2{perpendicular[0]/length,perpendicular[1]/length};
 std::cout<<std::setprecision(15)<<"projection="<<projection<<" z0="<<perpendicular[0]<<" z1="<<perpendicular[1]<<" q20="<<q2[0]<<" q21="<<q2[1]<<" orth="<<dot(q,q2)<<" norm2="<<dot(q2,q2)<<"\n";
 if(std::abs(perpendicular[0]-1)>1e-12||std::abs(perpendicular[1]+1)>1e-12||std::abs(dot(q,q2))>1e-12||std::abs(dot(q2,q2)-1)>1e-12)return 1;
 std::cout<<"PASS E07\n";
}
