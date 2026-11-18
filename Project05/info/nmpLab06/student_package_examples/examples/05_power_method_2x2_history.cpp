// E05: Fully worked power method for a separate 2x2 teaching matrix only.
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
using V=std::array<double,2>;
V mul(V x){return {2*x[0]+x[1],x[0]+2*x[1]};}
double dot(V x,V y){return x[0]*y[0]+x[1]*y[1];}
double rq(V x){return dot(x,mul(x))/dot(x,x);}
double res(V x){V y=mul(x);double lambda=rq(x);return std::hypot(y[0]-lambda*x[0],y[1]-lambda*x[1]);}
int main(int argc,char**argv){
 try{
 std::string path="data/power_history.csv";bool overwrite=false;
 for(int i=1;i<argc;i++){std::string arg=argv[i];if(arg=="--output"&&i+1<argc)path=argv[++i];else if(arg=="--overwrite")overwrite=true;else throw std::runtime_error("Usage: E05 [--output filename] [--overwrite]");}
 if(std::filesystem::exists(path)&&!overwrite)throw std::runtime_error("Output exists: use a new path or --overwrite");
 std::ofstream out(path,std::ios::trunc);if(!out)throw std::runtime_error("Cannot open output: "+path);
 out<<std::setprecision(17)<<"iteration,rayleigh,residual_norm\n";
 V x{1,0}; double prior_res=2;
 for(int k=0;k<=18;k++){
  double l=rq(x),r=res(x);if(!std::isfinite(r)||!std::isfinite(l)||r>prior_res+1e-12)throw std::runtime_error("Residual increased unexpectedly for this toy");
  out<<k<<','<<l<<','<<r<<'\n';prior_res=r;
  if(k==0||k==1||k==2||k==18)std::cout<<std::setprecision(15)<<"iteration="<<k<<" rayleigh="<<l<<" residual="<<r<<"\n";
  if(k<18){V y=mul(x);double norm=std::hypot(y[0],y[1]);if(norm<=0||!std::isfinite(norm))throw std::runtime_error("Zero or nonfinite norm");x={y[0]/norm,y[1]/norm};}
 }
 if(std::abs(rq(x)-3)>1e-12||res(x)>1e-7||std::abs(std::abs(dot(x,{1/std::sqrt(2.),1/std::sqrt(2.)}))-1)>1e-9)throw std::runtime_error("Eigenpair mismatch");
 std::cout<<"PASS E05\n";
 }catch(const std::exception&e){std::cerr<<"ERROR: "<<e.what()<<'\n';return 1;}
}
