#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "matops.h"
#include "solvers.h"


/* int main(){ */

/*  /\*  double rowmjrarrayam[]={  2, 1,-1, 8 *\/ */
/*  /\* 			  ,-3,-1, 2,-11 *\/ */
/*  /\* 			  ,-2, 1, 2,-3}; *\/ */
/*  /\*  structmatrix am=creatematrix(rowmjrarrayam *\/ */
/*  /\* 			      ,3,4 *\/ */
/*  /\* 			      ,endt(MATDOUBLE),rowmjr); *\/ */

/*  /\* //test gauss *\/ */
/*  /\*  gaussop(&am); *\/ */
/*  /\*  structmatrix solns=backsolve(&am); *\/ */
/*  /\*  printmat(&am); *\/ */
/*  /\*  printmat(&solns); *\/ */




/* /\*   double rowmjrarraya[]={   2, 1,-1 *\/ */
/* /\* 			  ,-3,-1, 2 *\/ */
/* /\* 			  ,-2, 1, 2}; *\/ */
/* /\* structmatrix a=creatematrix(rowmjrarraya *\/ */
/* /\* 			      ,3,3 *\/ */
/* /\* 			      ,endt(MATDOUBLE),rowmjr); *\/ */
/* /\*  double rowmjrarrayb[]={ 8 *\/ */
/* /\* 			,-11 *\/ */
/* /\* 			,-3}; *\/ */
/* /\* structmatrix b=creatematrix(rowmjrarrayb *\/ */
/* /\* 			      ,3,1 *\/ */
/* /\* 			      ,endt(MATDOUBLE),rowmjr); *\/ */


/* //works for jacobi */
/*   double rowmjrarraya[]={   7, -2,1,2 */
/*   			    ,2,8, 3,1 */
/*   			    ,-1, 0, 5,2 */
/*   			    ,0,2,-1,4}; */
/*   structmatrix a=creatematrix(rowmjrarraya */
/*   			      ,4,4 */
/*   			      ,endt(MATDOUBLE),rowmjr); */
/*   double rowmjrarrayb[]={ 3 */
/*   			  ,-2 */
/*   			  ,5 */
/*   			  ,4}; */
/*   structmatrix b=creatematrix(rowmjrarrayb */
/*   			      ,4,1 */
/*   			      ,endt(MATDOUBLE),rowmjr); */


/*   structmatrix XG=gaussseidel(&a,&b); */
/*   printmat(&XG); */
/*   structmatrix XS=soverrelaxation(&a,&b); */
/*   printmat(&XS); */

/*   double acga[]={   4,-1, 1 */
/* 		  ,-1, 4,-2 */
/* 		  , 1,-2, 4}; */
/*   double bcga[]={ 12 */
/* 		 ,-1 */
/* 		 , 5}; */
/*   structmatrix acg=creatematrix(acga,3,3,endt(MATDOUBLE),rowmjr); */
/*   structmatrix bcg=creatematrix(bcga,3,1,endt(MATDOUBLE),rowmjr); */
/*   structmatrix XCG=conjugategradient(&acg,&bcg); */
/*   printmat(&XCG);//exact answer is 3,1,1 */


/*   return 0; */
/* } */


//todo check sqr mtrx
void *gaussop(structmatrix *AM){//containts the rhs and lhs. in place
  fpidx idx=getidxingfunc(AM);//(r,c,mat)
#define av(ri,ci) *(double*) idx(&ri,&ci,AM)
 
 unsigned int ci;
  for(ci=0;ci<AM->nrows;ci++){
    //find pivot for col
    //findmax
    unsigned int imax;
    double maxabsval=-DBL_MAX,absv;
    for(imax=ci;imax<AM->nrows;imax++){
      absv=abs(av(imax,ci));
      if(absv>maxabsval){maxabsval=absv; continue;}
      else{continue;}
    }
    //printf("max in col %d %lf",ci,maxabsval);
    //argmax
    for(imax=ci;imax<AM->nrows;imax++){
      if(maxabsval==abs(av(imax,ci))){break;}
    }
    //printf("max ri %d \n",imax);
    swaprows(ci,imax,AM);

    unsigned int i;
    for(i=ci+1;i<AM->nrows;i++){
      unsigned int j;
      for(j=ci+1;j<AM->ncols;j++){
    	av(i,j)=av(i,j)-av(ci,j)*(av(i,ci)/av(ci,ci));
      }
      av(i,ci)=0;//fill lower triangle with zeros
    }

  }
#undef av
}


void *swaprows(unsigned int m1, unsigned int m2, structmatrix *m){ 
fpidx idx=getidxingfunc(m);//(r,c,mat)
#define av(ri,ci) *(double*) idx(&ri,&ci,m)
    unsigned int nci;
    for(nci=0;nci<m->ncols;nci++){
      double vc1,vc2;
      vc1=av(m1,nci);vc2=av(m2,nci);
      av(m2,nci)=vc1;av(m1,nci)=vc2;
    }
#undef av
}

static structmatrix backsolve(structmatrix *echform){
  structmatrix solnm=creatematrix(malloc(sizeof(double)*echform->nrows)
				  ,echform->nrows,1//solns in col vector
				  ,endt(MATDOUBLE),rowmjr);
  unsigned int zero=0,lci=echform->ncols-1;
  fpidx idxe=getidxingfunc(echform);//(r,c,mat)
  fpidx idxm=getidxingfunc(&solnm);
#define ave(ri,ci) *(double*) idxe(&ri,&ci,echform)
#define avs(ri,ci) *(double*) idxm(&ri,&ci,&solnm)
  unsigned int ri;
  for(ri=echform->nrows-1;ri<-1;ri--){
    double rowsum=0;unsigned int ci=0;
    for(ci=ri+1;ci<echform->nrows;ci++){
      ;rowsum=rowsum+(ave(ri,ci))*(avs(ci,zero));}
    avs(ri,zero)=(ave(ri,lci)-rowsum)/ave(ri,ri);
  }
  return solnm;
#undef ave
#undef avs
}




structmatrix jacobi(structmatrix *A, structmatrix *b){
  const unsigned int MAXITER=100;
  const double TOL=1e-6;
  double *xp=calloc(A->nrows,sizeof(double));//init guess 0s
  double *x= calloc(A->nrows,sizeof(double));//
  structmatrix XP=creatematrix(xp
			       ,A->nrows,1//solns in col vector
			       ,endt(MATDOUBLE),rowmjr);
  structmatrix X=creatematrix(x
			      ,A->nrows,1//solns in col vector
			      ,endt(MATDOUBLE),rowmjr);
  fpidx idxA= getidxingfunc( A);
  fpidx idxb= getidxingfunc( b);
  fpidx idxX= getidxingfunc(&X);
  fpidx idxXP=getidxingfunc(&XP);
#define Xv(ri,ci) *(double*)  idxX( &ri,&ci,&X)
#define XPv(ri,ci) *(double*) idxXP(&ri,&ci,&XP)
#define Av(ri,ci) *(double*)  idxA( &ri,&ci, A)
#define bv(ri,ci) *(double*)  idxb( &ri,&ci, b)
 unsigned int zero=0;

  unsigned int k=0;
  while(k<MAXITER){

    unsigned ri;
    for(ri=0;ri<A->nrows;ri++){
      double sumsoln=0;unsigned int sumi=0;
      for(sumi=0;(sumi<XP.nrows);sumi++)
	{if(sumi==ri){continue;};
	  sumsoln=sumsoln-Av(ri,sumi)*XPv(sumi,zero);}
      Xv(ri,zero)=(1/Av(ri,ri))*(sumsoln+bv(ri,zero));
    }

    double normofdiff=0;
    for(ri=0;ri<X.nrows;ri++){
      normofdiff+=pow((Xv(ri,zero)-XPv(ri,zero)),2);}
    normofdiff=pow(normofdiff,.5);

    if(normofdiff<TOL){printf("converged in %d iterations\n",k+1);return X;}

    for(ri=0;ri<X.nrows;ri++){XPv(ri,zero)=Xv(ri,zero);}

    k++;  }

  free(XP.data);
  return X;
#undef Xv
#undef XPv
#undef Av
#undef bv
}



structmatrix gaussseidel(structmatrix *A, structmatrix *b){
  const unsigned int MAXITER=100;
  const double TOL=1e-6;
  double *xp=calloc(A->nrows,sizeof(double));//init guess 0s
  double *x= calloc(A->nrows,sizeof(double));//
  structmatrix XP=creatematrix(xp
			       ,A->nrows,1//solns in col vector
			       ,endt(MATDOUBLE),rowmjr);
  structmatrix X=creatematrix(x
			      ,A->nrows,1//solns in col vector
			      ,endt(MATDOUBLE),rowmjr);
  fpidx idxA= getidxingfunc( A);
  fpidx idxb= getidxingfunc( b);
  fpidx idxX= getidxingfunc(&X);
  fpidx idxXP=getidxingfunc(&XP);
#define Xv(ri,ci) *(double*)  idxX( &ri,&ci,&X)
#define XPv(ri,ci) *(double*) idxXP(&ri,&ci,&XP)
#define Av(ri,ci) *(double*)  idxA( &ri,&ci, A)
#define bv(ri,ci) *(double*)  idxb( &ri,&ci, b)
 unsigned int zero=0;

  unsigned int k=0;
  while(k<MAXITER){

    unsigned ri;
    for(ri=0;ri<A->nrows;ri++){
      double newsumsoln=0;unsigned int sumi=0;
      for(sumi=0;(sumi<(ri));sumi++)//tricky iter
	{newsumsoln=newsumsoln-Av(ri,sumi)*Xv(sumi,zero);}
      double oldsumsoln=0;
      for(sumi=ri+1;(sumi<A->nrows);sumi++)//memory use reduction opportunity here
	{oldsumsoln=oldsumsoln-Av(ri,sumi)*XPv(sumi,zero);}
      Xv(ri,zero)=(1/Av(ri,ri))*(newsumsoln+oldsumsoln+bv(ri,zero));
    }

    double normofdiff=0;
    for(ri=0;ri<X.nrows;ri++){
      normofdiff+=pow((Xv(ri,zero)-XPv(ri,zero)),2);}
    normofdiff=pow(normofdiff,.5);

    if(normofdiff<TOL){printf("converged in %d iterations\n",k+1);return X;}

    for(ri=0;ri<X.nrows;ri++){XPv(ri,zero)=Xv(ri,zero);}

    k++;  }

  free(XP.data);
  return X;
#undef Xv
#undef XPv
#undef Av
#undef bv
}


structmatrix soverrelaxation(structmatrix *A, structmatrix *b){
  const unsigned int MAXITER=100;
  const double TOL=1e-6;
  double *xp=calloc(A->nrows,sizeof(double));//init guess 0s
  double *x= calloc(A->nrows,sizeof(double));//
  structmatrix XP=creatematrix(xp
			       ,A->nrows,1//solns in col vector
			       ,endt(MATDOUBLE),rowmjr);
  structmatrix X=creatematrix(x
			      ,A->nrows,1//solns in col vector
			      ,endt(MATDOUBLE),rowmjr);
  fpidx idxA= getidxingfunc( A);
  fpidx idxb= getidxingfunc( b);
  fpidx idxX= getidxingfunc(&X);
  fpidx idxXP=getidxingfunc(&XP);
#define Xv(ri,ci) *(double*)  idxX( &ri,&ci,&X)
#define XPv(ri,ci) *(double*) idxXP(&ri,&ci,&XP)
#define Av(ri,ci) *(double*)  idxA( &ri,&ci, A)
#define bv(ri,ci) *(double*)  idxb( &ri,&ci, b)
 unsigned int zero=0;


 double omega=.5;//b/w 0 and 2
  unsigned int k=0;
  while(k<MAXITER){

    unsigned ri;
    for(ri=0;ri<A->nrows;ri++){
      double newsumsoln=0;unsigned int sumi=0;
      for(sumi=0;(sumi<(ri));sumi++)//tricky iter
	{newsumsoln=newsumsoln-Av(ri,sumi)*Xv(sumi,zero);}
      double oldsumsoln=0;
      for(sumi=ri+1;(sumi<A->nrows);sumi++)
	{oldsumsoln=oldsumsoln-Av(ri,sumi)*XPv(sumi,zero);}
      Xv(ri,zero)=(1/Av(ri,ri))*(newsumsoln+oldsumsoln+bv(ri,zero))*omega
	+(1-omega)*XPv(ri,zero);
    }


    double normofdiff=0;
    for(ri=0;ri<X.nrows;ri++){
      normofdiff+=pow((Xv(ri,zero)-XPv(ri,zero)),2);}
    normofdiff=pow(normofdiff,.5);

    if(normofdiff<TOL){printf("converged in %d iterations\n",k+1);return X;}

    for(ri=0;ri<X.nrows;ri++){XPv(ri,zero)=Xv(ri,zero);}

    k++;  }

  free(XP.data);
  return X;
#undef Xv
#undef XPv
#undef Av
#undef bv
}

//todo put convergence into one function
structmatrix conjugategradient(structmatrix *A,structmatrix *b){
 const unsigned int MAXITER=100;
  const MATDOUBLE TOL=1e-6;
  MATDOUBLE *xp=calloc(A->nrows,sizeof(MATDOUBLE));//init guess 0s
  MATDOUBLE *x= malloc(A->nrows*sizeof(MATDOUBLE));//
  structmatrix XP=creatematrix(xp
			       ,A->nrows,1//solns in col vector
			       ,endt(MATDOUBLE),rowmjr);
  structmatrix X=creatematrix(x
			      ,A->nrows,1//solns in col vector
			      ,endt(MATDOUBLE),rowmjr);
  structmatrix rP=copymatrix(b); //previous error
  structmatrix pP=copymatrix(b); //CG vec
  structmatrix r=creatematrix(malloc(sizeof(MATDOUBLE)*rP.nrows)
			      ,rP.nrows,1
			      ,endt(MATDOUBLE),rowmjr);
  structmatrix p=creatematrix(malloc(sizeof(MATDOUBLE)*pP.nrows)
			      ,pP.nrows,1
			      ,endt(MATDOUBLE),rowmjr);
  fpidx idxA= getidxingfunc( A);
  fpidx idxb= getidxingfunc( b);
  fpidx idxX= getidxingfunc(&X);
  fpidx idxXP=getidxingfunc(&XP);
  fpidx idxrP=getidxingfunc(&rP);
  fpidx idxpP=getidxingfunc(&pP);
  fpidx idxr =getidxingfunc(&r );
  fpidx idxp =getidxingfunc(&p );
#define Xv(ri,ci)  *(double*)  idxX( &ri,&ci,&X)
#define XPv(ri,ci)  *(double*) idxXP(&ri,&ci,&XP)
#define Av(ri,ci)  *(double*)  idxA( &ri,&ci, A)
#define bv(ri,ci)  *(double*)  idxb( &ri,&ci, b)
#define rPv(ri,ci) *(double*)  idxrP( &ri,&ci, &rP)
#define pPv(ri,ci)  *(double*)  idxb( &ri,&ci, &pP)
#define rv(ri,ci)  *(double*)  idxA(  &ri,&ci, &r)
#define pv(ri,ci)  *(double*)  idxb( &ri,&ci,  &p)

  //result definition
#define ApPv(ri,ci) *(double*) idxApP(&ri,&ci,&ApP)

 unsigned int zero=0;

  unsigned int k=0;
  while(k<MAXITER){ 
    double alpha, beta;//scalars to compute in each interatin

    structmatrix rPT=(vecT(&rP)),pPT=vecT(&pP);
    structmatrix rPTrP=matrixmatrixmuldbl(&rPT,&rP);//1x1
    structmatrix ApP=matrixmatrixmuldbl(A,&pP);// vec nrows
    fpidx idxApP =getidxingfunc(&ApP);
    double alphan=*(double*) rPTrP.data;
    double alphad=*(double*) (matrixmatrixmuldbl(&pPT,&ApP)).data;
    alpha=alphan/alphad;//alpha new
    unsigned int ri;
    for(ri=0;ri<XP.nrows;ri++){
      Xv(ri,zero)=XPv(ri,zero)+alpha*pPv(ri,zero); //new x approx soln
      rv(ri,zero)=rPv(ri,zero)-alpha*ApPv(ri,zero);//new r error
    }
    structmatrix rT=vecT(&r);
    structmatrix rTr=matrixmatrixmuldbl(&rT,&r);
    beta=(*(double*) rTr.data)/(*(double*) rPTrP.data); //scalar/scalar
    for(ri=0;ri<p.nrows;ri++){
      pv(ri,zero)=rv(ri,zero)+beta*pPv(ri,zero);
    }
    double normofr=0;
    for(ri=0;ri<X.nrows;ri++){normofr+=pow(rv(ri,zero),2);}
    normofr=pow(normofr,.5);

    if(normofr<TOL){printf("converged\n in %d iterations\n",k+1);return X;}

    //previous=current
    for(ri=0;ri<X.nrows;ri++){
      XPv(ri,zero)=Xv(ri,zero);
      pPv(ri,zero)=pv(ri,zero);
      rPv(ri,zero)=rv(ri,zero);
    }

    free(rPT.data);
    free(rPTrP.data);
    free(ApP.data);
    free(rT.data);
    free(rTr.data);
    k++;}

  free(XP.data);
  free(rP.data);
  free(pP.data);
  free(r.data);
  free(p.data);
  return X;
#undef Xv
#undef XPv
#undef Av
#undef bv
#undef rPv
#undef pPv
#undef rv
#undef pv
#undef ApPv
}


static structmatrix vecT(structmatrix *vec){
  structmatrix vT=*vec;//does not copy data
  vT.nrows=vec->ncols;
  vT.ncols=vec->nrows;
  return vT;
}

static void *mulmatbyconst(double *constv,structmatrix *m){
  unsigned int i,len=(m->nrows*m->ncols);
  for(i=0;i<len;i++){*(double*) (m->data+i*sizeof(double))*=(*constv);}
  return;
}
