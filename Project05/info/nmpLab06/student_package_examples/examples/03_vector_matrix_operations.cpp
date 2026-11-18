// E03: Dot product, Euclidean norm, normalization and a 3x3 matrix-vector product.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
using V=std::array<double,3>;
double dot(const V&a,const V&b){double s=0;for(int i=0;i<3;i++)s+=a[i]*b[i];return s;}
double norm(const V&v){return std::sqrt(dot(v,v));}
V normalize(V v){double n=norm(v);if(!(n>0)||!std::isfinite(n))throw std::runtime_error("bad norm");for(double &z:v)z/=n;return v;}
int main(){
 V u{1,2,3},w{4,-1,2}; std::array<std::array<double,3>,3> T{{{{-2,1,0}},{{1,-2,1}},{{0,1,-2}}}};
 V y{};for(int i=0;i<3;i++)for(int j=0;j<3;j++)y[i]+=T[i][j]*u[j];
 std::cout<<std::setprecision(15)<<"dot="<<dot(u,w)<<" norm_u="<<norm(u)<<" norm_normalized="<<norm(normalize(u))<<"\n";
 for(int i=0;i<3;i++)std::cout<<"y"<<i<<"="<<y[i]<<"\n";
 if(std::abs(dot(u,w)-8)>1e-12||std::abs(norm(u)-std::sqrt(14.))>1e-12||std::abs(norm(normalize(u))-1)>1e-12||y!=V{0,0,-4})return 1;
 std::cout<<"PASS E03\n";
}
