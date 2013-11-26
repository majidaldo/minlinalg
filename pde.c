#include <stdio.h>
#include "matops.h"
#include "solvers.h"


//2D to 1D conversions
unsigned int up   (unsigned int *i, unsigned int *j);
unsigned int down (unsigned int *i, unsigned int *j);
unsigned int left (unsigned int *i, unsigned int *j);
unsigned int right(unsigned int *i, unsigned int *j);
unsigned int ki   (unsigned int *i, unsigned int *j);

void *fillA(structmatrix *A);
void *fillb(structmatrix *b);

double kappa;

unsigned int nt;
double tmax,dt;

unsigned int ny,nx;
double Lx,Ly;
double dx, dy;

double sx,sy;

double qmax;

int main(){

  kappa=1;
  nt=3,tmax=1000;//change to a big number and you can see a convergence
  dt=tmax/(nt-1.0);//i want to const these vars todo

  nx=20,ny=10;
  Lx=2.0,Ly=1.0;
  dx=Lx/(nx-1.0),dy=Ly/(ny-1.0);
  //central difference scheme. need at least 2 segments (3 nodes)

  sx=kappa*dt/(dx*dx),sy=kappa*dt/(dy*dy);

  qmax=100;

  structmatrix  A=creatematrix(calloc(nx*ny*nx*ny,sizeof(MATDOUBLE))
			       ,nx*ny,nx*ny
			       ,endt(MATDOUBLE)
			       ,rowmjr
			       );
  structmatrix  b=creatematrix(calloc(nx*ny,sizeof(MATDOUBLE))
			       ,nx*ny,1
			       ,endt(MATDOUBLE)
			       ,rowmjr
			       );
  fillA(&A);fillb(&b);
  structmatrix x=soverrelaxation(&A,&b);
  //printmat(&A);printmat(&b);
  printmat(&x);

}





unsigned int ki(unsigned int *i, unsigned int *j){return nx*(*j)+*i;}
unsigned int up(unsigned int *i, unsigned int *j){
  unsigned int jp1=*j+1;return ki(i,&jp1) ;}
unsigned int down(unsigned int *i, unsigned int *j){
  unsigned int jm1=*j-1;return ki(i,&jm1) ;}
unsigned int right(unsigned int *i, unsigned int *j){
  unsigned int ip1=*i+1;return ki(&ip1,j) ;}
unsigned int left(unsigned int *i, unsigned int *j){
  unsigned int im1=*i-1;return ki(&im1,j) ;}


void *fillA(structmatrix *A){

fpidx idxA= getidxingfunc( A);
#define Av(ri,ci) *(double*)  idxA( &ri,&ci,A)

  //fill interior nodes coeff
 unsigned int i,j,k,kk;
 for(i=1;i<(nx-1);i++){
   for(j=1;j<(ny-1);j++){
      //center
      k=ki(&i,&j);
      Av(k,k)=1+2*sx+2*sy;
      //up
      kk=up(&i,&j);
      Av(k,kk)=-sy;
      //down
      kk=down(&i,&j);
      Av(k,kk)=-sy;
      //right
      kk=right(&i,&j);
      Av(k,kk)=-sx;
      //left
      kk=left(&i,&j);
      Av(k,kk)=-sx;
    }
  }
 //edges can be readily known
 i=0;   for(j=0;j< ny   ;j++){k=ki(&i,&j);assert(Av(k,k)==0);Av(k,k)=1;}//left
 j=0;   for(i=1;i<(nx  );i++){k=ki(&i,&j);assert(Av(k,k)==0);Av(k,k)=1;}//bottom
 i=nx-1;for(j=1;j< ny   ;j++){k=ki(&i,&j);kk=left(&i,&j);assert(Av(k,k)==0);//right
  //right edge=closest "column" (zero slope at right edge)
   Av(k,kk)=1;Av(k,k)=-1;}
 j=ny-1;for(i=1;i<(nx-1);i++){k=ki(&i,&j);kk=down(&i,&j);assert(Av(k,k)==0);//top
   Av(k,kk)=1;Av(k,k)=-1;}//top edge = closest "row" (slope=0 at top)
 //check how many ones (should equal num of perimiter nodes+neighbor nodes
 /* kk=0;for(i=0;i<(nx*ny);i++){for(j=0;j<(ny*nx);j++){ */
 /*     ;if(Av(i,j)==1 || Av(i,j)==-1){kk+=1;}}} */
 /*     printf("\n\n1ct %d",kk); */

#undef Av
}



void *fillb(structmatrix *b){

fpidx idxb= getidxingfunc(b);
#define bv(ri,ci) *(double*)  idxb( &ri,&ci,b)
 unsigned int zero=0;//const gives a warning

 unsigned int i,j,k;
 //left profile
 i=0;for(j=0;j<ny;j++){
   k=ki(&i,&j);assert(bv(k,zero)==0);
   bv(k,zero)=(4*qmax/Ly)*(dy*j-(dy*j*dy*j)/Ly);
 }
 //bottom profile the same just to make things intersting
 j=0;for(i=0;i<(nx);i++){
   k=ki(&i,&j);assert(bv(k,zero)==0);
   //assert ok even at i,j=0,0 b/c it's a profile with 0 at the ends
   bv(k,zero)=(4*qmax/Lx)*(dx*i-(dx*i*dx*i)/Lx);
 }

#undef bv
}
