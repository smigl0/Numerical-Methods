// E01: Verify known eigenpairs of a symmetric 2x2 matrix.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
using V=std::array<double,2>;
V mul(V x){return {2*x[0]+x[1],x[0]+2*x[1]};}
double dot(V a,V b){return a[0]*b[0]+a[1]*b[1];}
double residual(double lambda,V q){V z=mul(q);return std::hypot(z[0]-lambda*q[0],z[1]-lambda*q[1]);}
int main(){
 const double s=1.0/std::sqrt(2.0); const V q1{s,s},q2{s,-s};
 const double r1=residual(3,q1),r2=residual(1,q2),orth=dot(q1,q2);
 std::cout<<std::setprecision(15)<<"lambda1=3 lambda2=1 residual1="<<r1<<" residual2="<<r2<<" orth="<<orth<<"\n";
 if(r1>1e-13||r2>1e-13||std::abs(orth)>1e-13||std::abs(dot(q1,q1)-1)>1e-13)return 1;
 std::cout<<"PASS E01\n";
}
