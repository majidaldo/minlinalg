
#include <stdio.h>
#include <stdbool.h>
#include <complex.h>


/*this serves to abstract away the matrix representation
i just care about the index
*/



#define MATREAL    float
#define MATDOUBLE  double
#define MATCOMPLEX complex
#define MATINT     int
#define MATBOOL    bool


//attribs are a set of properties

#define matrep attribs[0]
typedef enum  { rowmjr, colmjr,constm} matrepnum;

#define matdtype attribs[1]
typedef enum { treal, tdouble, tcomplex, tint, tbool} dtypenum;
size_t sizeofmatdtype[]={//match enum
  sizeof(float),sizeof(double),sizeof(complex),sizeof(int),sizeof(bool)
};



//i cant help but think of multidimensional arrays as just arrays in arrays
//but i will do matrix..yawn.
typedef struct structmatrix {
  void *data;//just an array
  unsigned int nrows,ncols;
  unsigned int attribs[2];//<-make sure to change this num with more properties
  //dtypenum datatype;
  struct structmatrix *parent;
} structmatrix;


typedef  void* (*fpidx)(unsigned int *ri, unsigned int *ci, structmatrix *amat);
fpidx getidxingfunc(structmatrix *amat);
void *idxasrowmjr(unsigned int *ri, unsigned int *ci, structmatrix *mat);
void *idxascolmjr(unsigned int *ri, unsigned int *ci, structmatrix *mat);
void *idxasconstm(unsigned int *ri,unsigned int *ci, structmatrix *aconstmat);

typedef void* (*fpop)(void*,void*);
void *elem2elem(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);

structmatrix initmatrix(void *data
			,unsigned int nrows,unsigned int ncols
			,unsigned int attribs[]//will this copy?
			);

structmatrix copymatrix(structmatrix *srcmat,structmatrix *dstmat);
//when copying the new mat will have no parent. the data will copy.
//if startptr same as end ptr ..(assert other props the same) memcpy
void *freematrixdata(structmatrix *amat);//if im not the parent then ok


structmatrix covert2double(structmatrix *nondblmat);
structmatrix transpose(structmatrix *amat);//if rowmjr set as colmjr and vice versa

void *submatrix(structmatrix *bigmat,structmatrix *smlmat);//just shift
//init a mat will null


//ops
#define castopfp2(dtypesrc,dtypedst,name)\
  (void* (*name)(dtypesrc *src, dtypedst *dst))	\
  (void *(*)(void*,void*))


void *muldoubles(double *src, double *dst);
void *adddoubles(double *src, double *dst);


int main(){

  //castopfp2(double,double,fp);

  // typedef  void *(*dblfp)(double *src, double *dst);
  //(void*) 
  // (void *(*)(void*,void*));


double td[]={111,222,333,444,555,666,777,888};
int ti[]={111,222,333,444,555,666,777,888};

structmatrix tm;
tm.data=td;
tm.ncols=2;tm.nrows=4;
tm.matrep=rowmjr;tm.matdtype=tdouble;


unsigned int tr=3,tc=0;


 elem2elem(&tm,&tm,(fpop) adddoubles );

printf("\natos%lf",*(double*) idxasrowmjr(&tr,&tc,&tm));
}


void *adddoubles(double *src, double *dst){*dst=*src+*dst;}


/* #define declarep2idxingfunc(name)\ */
/*   void* (*name)(unsigned int *ri, unsigned int *ci, structmatrix *amat) */

void *elem2elem(structmatrix *srcmat,structmatrix *amat,fpop fp2op){

// declarep2idxingfunc(ifp);
  fpidx ifp=getidxingfunc(amat);

 unsigned int ri,ci;
 for(ri=0;ri<(amat->nrows);ri++){
   for(ci=0;ci<(amat->ncols);ci++){
     fp2op(ifp(&ri,&ci,amat),ifp(&ri,&ci,amat));//(src,dst)
     //ifp returns address of element to be processed by fp2op
   }}
}



fpidx getidxingfunc(structmatrix *amat){
  if     (amat->matrep==rowmjr){return &idxasrowmjr;}
  else if(amat->matrep==colmjr){return &idxascolmjr;}
  else                         {return &idxasconstm;}
}
void *idxasconstm(unsigned int *ri,unsigned int *ci, structmatrix *aconstmat){
  return aconstmat->data;
}
void *idxasrowmjr(unsigned int *ri, unsigned int *ci, structmatrix *amat){
  //could errorcheck here
#define poffset sizeofmatdtype[amat->matdtype]*((*ri)*(amat->ncols)+(*ci))
return amat->data+poffset;
#undef poffset
}
void *idxascolmjr(unsigned int *ri, unsigned int *ci, structmatrix *amat){
  //could errorcheck here
#define poffset sizeofmatdtype[amat->matdtype]*((*ri)+(amat->nrows)*(*ci))
  return amat->data+poffset;
#undef poffset
}



