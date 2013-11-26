#include "matops.h"

//int main(){}

/* int main(){ */


/* double td[]={111,222,333,444,555,666,777,888}; */
/* int ti[]={111,222,333,444,555,666,777,888}; */

/* structmatrix tm; */
/* tm.data=td; */
/* tm.ncols=2;tm.nrows=4; */
/*  tm.matrep=rowmjr;tm.matdtype=endt(MATDOUBLE); */


/* unsigned int tr=3,tc=0;  */
/* /\*  opone2one(&tm,&tm,(fpop) adddoubles ); *\/ */
/* /\* printf("\n op1 %lf",*(MATDOUBLE*) idxasrowmjr(&tr,&tc,&tm)); *\/ */

/* //arrayop(&tm,&tm,elemadd,one2one);//problem this isn't doing anyhting */
/* //printf("\n op2 %lf",*(MATDOUBLE*) idxasrowmjr(&tr,&tc,&tm)); */
/*  printmat(&tm); */

/* } */




structmatrix matrixmatrixmuldbl(structmatrix *A,structmatrix *B){
  if(A->ncols!=B->nrows)
    {printf("incompatible matrices for multiplication");exit(1);}

  structmatrix C=creatematrix(malloc(sizeof(double)*A->nrows*B->ncols)
			      ,A->nrows,B->ncols
			      ,endt(MATDOUBLE), rowmjr);
  fpidx idxB= getidxingfunc( B);
  fpidx idxA= getidxingfunc( A);
  fpidx idxC= getidxingfunc(&C);
#define  Bv(ri,ci) *(double*) idxB(&ri,&ci, B)
#define  Av(ri,ci) *(double*) idxA(&ri,&ci, A)
#define  Cv(ri,ci) *(double*) idxC(&ri,&ci,&C)


unsigned int ri,ci;
for(ri=0;ri<C.nrows;ri++){//for a row,col in product, it's the summation of
  for(ci=0;ci<C.ncols;ci++){//...row A times col B

    unsigned int ici;
    double sum=0;
    for(ici=0;ici<A->ncols;ici++){
      double a=Av(ri,ici),b=Bv(ci,ici);
      ;sum+=(Av(ri,ici)*Bv(ici,ci));}

    Cv(ri,ci)=sum;
  }
 }

return C;
}

//void *overcol(unsigned int *cn, structmatrix *amat)
//void *overrow(unsigned int *rn, structmatrix *amat);
//void *overdiag(unsigned int *dn, structmatrix *amat);

structmatrix creatematrix(void *datainmatrepform
			  ,unsigned int nrows,unsigned int ncols
			  ,dtypenum dtnum, matrepnum mrnum
			  ){
  structmatrix mc;
  mc.data=datainmatrepform;
  mc.nrows=nrows;mc.ncols=ncols;
  mc.matdtype=dtnum;mc.matrep=mrnum;
  return mc;
}

structmatrix copymatrix(structmatrix *src){
  //assuming the data was allocated with a malloc
  if(src->matrep==colmjr || src->matrep==rowmjr){
    void *data=malloc((src->ncols)*(src->nrows)*sizeofmatdtype[src->matdtype]);
    structmatrix mc=creatematrix(data
				 ,src->nrows,src->ncols
				 ,src->matdtype,src->matrep);
    arrayop(src,&mc,elemset,one2one);
    return mc;
  }
  else{printf("can't copy matrep num %d",src->matrep);exit(1);}
      
 
}


void *printmat(structmatrix *amat){
  if(amat->matdtype!=endt(MATDOUBLE))
    {fprintf(stderr,"don't know how to print matrix");return;}
  
  printf("rows=%d, cols=%d, repnum=%d, ddtypenum=%d\n"
	 ,amat->nrows,amat->ncols,amat->matrep,amat->matdtype);

  //todo break into blocks//const unsigned int maxcol=20;
  fpidx idxing= getidxingfunc(amat);
  unsigned int ci,ri;
  for(ri=0;ri<(amat->nrows);ri++){
    printf("row %d\n",ri);
    for(ci=0;ci<(amat->ncols);ci++){
      //printf("(%d%d)", ri ,ci);
      printf("%E ",*(MATDOUBLE*) idxing(&ri,&ci,amat));
    }
    printf("\n");
  }

}





void *arrayop(structmatrix *srcmat, structmatrix *dstmat
	       ,elemopnum opnum,matmapnum mapnum){
  fpop    opfp=getmatrixop     (srcmat,dstmat,opnum);
  fpmap  mapfp=getmatrixmapping(srcmat,dstmat,mapnum);
  mapfp(srcmat,dstmat,opfp);//apply operation to mapping
}





// todo static chk if indexed dst data does not correpond to a shift. 
//if it does not then block operation (mapping not compatible)
//its a function of mapping op and dst type and perhaps src type.
//first check smartly by blocking certain combos then brute force by
//gong through and iterating.

/*MAPPING FUNCTIONS*/

static fpmap getmatrixmapping(structmatrix *srcmat,structmatrix *dstmat
			      ,matmapnum mapnum){
  //check dimensions. it's a -mapping-
  if(mapnum==one2one){
    if(srcmat->ncols!=dstmat->ncols || srcmat->nrows!=dstmat->nrows){
      fprintf(stderr,"one2one op dimensions not compatible");}
    else{return &opone2one;}
  }
  else if(mapnum){} //todo
  //else if(mapnum==row2cols){return &opcol2row;}//todo check compat
  else if(true){printf("mapping not implemented");}
  assert(false);
}

//todo
static void *optranspose(structmatrix *srcmat, structmatrix *dstmat
		  ,fpop fp2op){
  //like one2one but switch dimensions
  fpidx sifp=getidxingfunc(srcmat),difp=getidxingfunc(dstmat);
  
  unsigned int ri,ci;
  for(ci=0;ci<(srcmat->ncols);ci++){
    for(ri=0;ri<(srcmat->nrows);ri++){
      fp2op(sifp(&ri,&ci,srcmat),difp(&ri,&ci,dstmat));//(src,dst)
      //ifp returns address of element to be processed by fp2op
    }
  }
}

static void *opone2one(structmatrix *srcmat,structmatrix *dstmat
		  ,fpop fp2op){
  fpidx sifp=getidxingfunc(srcmat),difp=getidxingfunc(dstmat);
  
  unsigned int ri,ci;
  for(ci=0;ci<(srcmat->ncols);ci++){
    for(ri=0;ri<(srcmat->nrows);ri++){
      fp2op(sifp(&ri,&ci,srcmat),difp(&ri,&ci,dstmat));//(src,dst)
      //ifp returns address of element to be processed by fp2op
    }
  }
}


//define in terms of one2one
/* void *opcol2rows(structmatrix *srcmat,structmatrix *dstmat */
/* 		,fpop fp2op){ */
/*   //apply one col to each row (apply same num to each row in dst) */
/*   fpidx sifp=getidxingfunc(srcmat),difp=getidxingfunc(dstmat); */
/*   //src and dst indexors and iterators */
/*   unsigned int si,di;//match col of src with row of dst */
/*   for(si=0;si<(srcmat->nrows);si++){ */
/*     for(di=0;di<(dstmat->ncols);di++){ */
/*       fp2op(ifp(&si,&(srcmat->nrows),srcmat) */
/* 	    ,ifp(&si,&di,dstmat)); */
/*     } */
/*   } */
/* } */




/*(MATHEMATICAL) INDEXING FUNCTIONS TO DATA*/

fpidx getidxingfunc(structmatrix *amat){
  if     (amat->matrep==rowmjr){return &idxasrowmjr;} //works even w/o &!
  else if(amat->matrep==colmjr){return &idxascolmjr;}
  else if(amat->matrep==constmr){return &idxasconstm;}
  assert(false);
}



void *idxasconstm(unsigned int *ri,unsigned int *ci, structmatrix *aconstmat){
  return aconstmat->data;
}
void *idxasrowmjr(unsigned int *ri, unsigned int *ci, structmatrix *amat){
  //could errorcheck here but the bound should be known
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
//todo safe indexing that checks.. made for the user




/* OPERATIONS TYPE CHECKING */

static fpop getmatrixop(structmatrix *srcmat,structmatrix *dstmat,elemopnum elemopnum){
  /*all datatype conversions and checking can be done here.
    should have nothing to do with dimenesions. these are just the arithmetic ops*/
  if(srcmat->matdtype!=endt(MATDOUBLE))
    {fprintf(stderr,"undefined datatype operation");exit(1);}//for all ops
  //return NULL;}//will not err handle
  else{
    if     (elemopnum==elemadd)  {return (fpop) &adddoubles;} 
    else if(elemopnum==elemmul)  {return (fpop) &muldoubles;}
    else if(elemopnum==elemset)  {return (fpop) &setdouble;}
    assert(false);
  }
assert(false);}

void *setdouble (MATDOUBLE *src, MATDOUBLE *dst){*dst=*src;}
void *adddoubles(MATDOUBLE *src, MATDOUBLE *dst){*dst=*src+*dst;}
void *muldoubles(MATDOUBLE *src, MATDOUBLE *dst){*dst=(*src)*(*dst);}
