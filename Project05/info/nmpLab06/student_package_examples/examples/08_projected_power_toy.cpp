// E08: Projected iteration on a separate 3x3 toy with first eigenvector GIVEN.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
using V=std::array<double,3>;
double dot(V x,V y){return x[0]*y[0]+x[1]*y[1]+x[2]*y[2];}
V mul(V x){return {3*x[0]+x[1],x[0]+3*x[1],x[2]};}
double norm(V x){return std::sqrt(dot(x,x));}
V project(V y,V q){double c=dot(y,q);for(int i=0;i<3;i++)y[i]-=c*q[i];return y;}
int main(){
 try{
 const double s=1/std::sqrt(2.);const V q1{s,s,0},q2{s,-s,0};V x{1,0,1};
 x=project(x,q1);for(double &z:x)z/=norm(x);
 for(int k=0;k<45;k++){
  V y=project(mul(x),q1);double len=norm(y);if(!(len>1e-14)||!std::isfinite(len))throw std::runtime_error("projected norm zero/nonfinite");
  for(int j=0;j<3;j++)x[j]=y[j]/len;
  if(k==0||k==4||k==44){double lambda=dot(x,mul(x));std::cout<<std::setprecision(15)<<"iteration="<<k+1<<" lambda="<<lambda<<" orth="<<dot(x,q1)<<"\n";}
 }
 V Ax=mul(x);double lambda=dot(x,Ax);V r{Ax[0]-lambda*x[0],Ax[1]-lambda*x[1],Ax[2]-lambda*x[2]};
 std::cout<<std::setprecision(15)<<"lambda="<<lambda<<" residual="<<norm(r)<<" orth="<<dot(x,q1)<<" alignment="<<std::abs(dot(x,q2))<<"\n";
 if(std::abs(lambda-2)>1e-10||norm(r)>1e-9||std::abs(dot(x,q1))>1e-12||std::abs(std::abs(dot(x,q2))-1)>1e-10)throw std::runtime_error("wrong projected eigenpair");
 std::cout<<"PASS E08\n";
 }catch(const std::exception &e){std::cerr<<"ERROR: "<<e.what()<<'\n';return 1;}
}
