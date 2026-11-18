// E09: Reconstruct B and B^{-1} from two EXACT toy eigenpairs.
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
using M=std::array<std::array<double,2>,2>;
int main(){
 double s=1/std::sqrt(2.);std::array<double,2> q1{s,s},q2{s,-s};M B{},Inv{};
 for(int i=0;i<2;i++)for(int j=0;j<2;j++){
  B[i][j]=3*q1[i]*q1[j]+q2[i]*q2[j];
  Inv[i][j]=q1[i]*q1[j]/3+q2[i]*q2[j];
 }
 M truth{{{{2,1}},{{1,2}}}},refInv{{{{2./3,-1./3}},{{-1./3,2./3}}}};
 double error=0,inv_error=0,id_error=0,symmetry=std::abs(Inv[0][1]-Inv[1][0]);
 for(int i=0;i<2;i++)for(int j=0;j<2;j++){
  error=std::max(error,std::abs(B[i][j]-truth[i][j]));inv_error=std::max(inv_error,std::abs(Inv[i][j]-refInv[i][j]));
  double e=0;for(int k=0;k<2;k++)e+=B[i][k]*Inv[k][j];id_error=std::max(id_error,std::abs(e-(i==j?1.:0.)));
 }
 std::cout<<std::setprecision(15)<<"B00="<<B[0][0]<<" B01="<<B[0][1]<<" inv00="<<Inv[0][0]<<" inv01="<<Inv[0][1]<<" recon_error="<<error<<" inverse_error="<<inv_error<<" identity_error="<<id_error<<" symmetry="<<symmetry<<"\n";
 if(error>1e-13||inv_error>1e-13||id_error>1e-13||symmetry>1e-13)return 1;
 std::cout<<"PASS E09\n";
}
