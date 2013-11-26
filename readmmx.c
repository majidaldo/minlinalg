#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for str funcs like strcmp
#include "mmtypes.h"
#include "utils.h"
#include "readingfuncs.h"
#include "readingfuncptrs.h"
#include <assert.h>
#include <stdint.h> //has datatype to store ptr as int
#include "readmmx.h"

/*
reads matrix market format files
*/








void *freemmdata(structmmdata *mmdata){
free((mmdata->p2data).value);
 free((mmdata->p2data).i1);free((mmdata->p2data).i2);
}


double *copyasdouble(unsigned int* mmirow,unsigned int* mmicol
			     ,structmmdata* mmdata){
#define NULLVAL 0
#define dt (mmdata->mmfileattribs).header.field
#define iffield(DTSTR) if(dt==indexof(DTSTR,field))
#define cpyfrom(dtstr)  {datacopy[i]=				\
    (double) *(dtstr*) *(uintptr_t*) p2mmvalsit.current;}

  structpiterator p2mmvalsit=getptrs2mmvec(mmirow,mmicol,mmdata);
  double *datacopy=malloc(sizeof(double)*p2mmvalsit.nobj);
 
 unsigned int i=0;
 while(getnextp(&p2mmvalsit)!=NULL){
   if ((void*) *(uintptr_t*)p2mmvalsit.current!=NULL){
          iffield(REAL)    cpyfrom(tmmreal)
     else iffield(DOUBLE)  cpyfrom(tmmdouble)
     else iffield(INTEGER) cpyfrom(tmmcomplex)
     else iffield(PATTERN) cpyfrom(tmmpattern)
    }
   else{datacopy[i]=NULLVAL;}
   i++;
 }

      return datacopy;

#undef NULLVAL
#undef dt
#undef iffield
#undef cpyfrom
}


/* void *oponvalat1(void *p2val, void* (*fptr)(void*) ) */
/* { return fptr(p2val);} */
/* void *oponvalat2(void *p2val, void *p2opobj,void* (*fptr)(void*,void*) ) */
/* { return fptr(p2val,p2opobj);} */


int printmmvalat(void *p2val, unsigned int* datatypenum){
   /*prints a the value of data stored at p2val*/
  if(p2val==NULL){return printf("NULL");}
#define iffield(DTSTR) if(*datatypenum==indexof(DTSTR,field))
#define print(fmt,dtype) {return printf(fmt, *(dtype*) p2val);}
  else{
    iffield(COMPLEX) {return printf("%f+%fi"
	,creal(*(tmmcomplex*)p2val),cimag(*(tmmcomplex*)p2val));}
    else iffield(REAL)    print("%f",tmmreal)
    else iffield(DOUBLE)  print("%lf",tmmdouble)
    else iffield(INTEGER) print("%d",tmmint)
    else iffield(PATTERN) {return printf("T");}
    else{assert(0);}
  }
#undef iffield
#undef print
}



structpiterator getptrs2mmvec(unsigned int *mmirow,unsigned int *mmicol
    ,structmmdata *mmdata){
  /*this is a selection function. to select a row mmrow, give *mmcol=0
    and vice versa.if both indexes are numbers you get array of length 1
    to the scalar. 0 for both gives the whole matrix
    make sure you select w/in bounds*/
#define sizeinfo(adim) (mmdata->mmfileattribs).size.adim
#define p2d(data) (mmdata->p2data).data
#define nc sizeinfo(ncols)
#define nr sizeinfo(nrows)
  if(*mmirow>nr || *mmicol>nc)
    {fprintf(stderr,"selected vector out of bounds");exit(1);}

  unsigned int ncolelems=1,nrowelems=1;//for malloc
  if(*mmirow==0){ncolelems=nr;}//i want to select a col,a col has nrows elems
  if(*mmicol==0){nrowelems=nc;} //i want to select a row
unsigned int nelems=ncolelems*nrowelems;
  //void **p2vals=malloc(nelems*sizeof(void*));//could not deref ptr to void*
  uintptr_t *p2vals=malloc(nelems*sizeof(uintptr_t));
  structpiterator p2pit;
  p2pit=initpiterator(p2vals,sizeof(uintptr_t),nelems);

  unsigned int mmri=1,mmci=1;//iterators over MM data
  unsigned int mmrj=1,mmcj=1;//incrementors (=j=umper)
  /*the above, unaltered, allows iteration over all MM elems.
    need to jumpt to index and block iteration if row and/or col selected*/
  //col selected,block col iter
  unsigned int coliend=nc,rowiend=nr;
  if(*mmicol!=0){mmcj=0;mmci=*mmicol;coliend=mmci;}
  if(*mmirow!=0){mmrj=0;mmri=*mmirow;rowiend=mmri;}
  //selected a row so block row iterator
  unsigned int mmristart=mmri,mmcistart=mmci;
  for(mmri=mmristart;mmri<=rowiend;mmri=mmri+mmrj){
    for(mmci=mmcistart;mmci<=coliend;mmci=mmci+mmcj){
      getnextp(&p2pit);
      *(uintptr_t*) p2pit.current=(uintptr_t) getp2valueat(&mmri,&mmci,mmdata);
      /* //how to go from ptr to void to array of ptrs to ptrs to datatype */
      /*  if(*(uintptr_t*) p2pit.current!=(uintptr_t) NULL){  */
      /* printf("\n at %d %d %d",mmri,mmci,*(tmmint*) *(uintptr_t*) p2pit.current);} */
      /* else{printf("\n at %d %d null",mmri,mmci);}; */
      if(*mmicol!=0){break;}
    }
    if(*mmirow!=0){break;}
  }

 reinitpiterator(&p2pit);
  return p2pit;//p2vals;
#undef p2d
#undef sizeinfo
#undef nc
#undef nr
}



void *getp2valueat(unsigned int *mmirow, unsigned int *mmicol, structmmdata *mmdata){
  //*returning data according to matrix market. MM indexing starts at 1.
#define sizeinfo(att) ((mmdata->mmfileattribs).size).att
#define nz sizeinfo(nonzeros)
#define nr sizeinfo(nrows)
#define nc sizeinfo(ncols)
#define p2d(data) (mmdata->p2data).data
#define dt (mmdata->mmfileattribs).header.field
#define mmfmt ((mmdata->mmfileattribs).header.format)

  if(*mmirow<1 || *mmicol<1 || *mmirow>nr || *mmicol>nc)
    //{return NULL;}
    {fprintf(stderr,"selected index out of bounds");exit(1);}

  unsigned int i=0;
  if( mmfmt==indexof(COORDINATE,format) ){
    //unsigned int inz; why can't this be here??
    for(i=0;i<nz;i++){
      if((p2d(i1)[i]==*mmirow) && (p2d(i2)[i]==*mmicol))
	{return p2d(value)+i*sizeoftype[dt];}
      else{continue;}
    }
    return NULL;//no value stored. if it was a pattern you get true;
  }
  else{assert(mmfmt==indexof(ARRAY,format));//MM is arranged by columns
    for(i=0;i<(nr*nc);i++){
      return p2d(value)+sizeoftype[dt]*( (*mmicol-1)*(nr)+(*mmirow-1) );
      //jump array by number of rows to get to a column
    }
  }

#undef mmfmt
#undef dt
#undef p2d
#undef sizeinfo
#undef nz
#undef nr
#undef nc
}



structmmdata readmmfile(FILE *pfile){
 
  structmmdata mmdata;
#define mmfatr mmdata.mmfileattribs

  if (pfile==NULL){
    printf("couldn't open file \n");
    exit(1);
  }
  else{
    //printf("file open success\n");
    unsigned int aerr=putfileattribs(pfile,&mmfatr);
    if(aerr!=0){printf("file formatting error code %d:",aerr);exit(1);}
    /* printf("\n attribs errorc: %d \n",aerr); */
  
#define p2data mmdata.p2data
    unsigned int len=putp2data(&mmfatr,&p2data);
   
    tmmindex     *pi1=  p2data.i1;
    tmmindex     *pi2=  p2data.i2;
    tmmreal      *pr=   p2data.value;
    tmmdouble    *pd=   p2data.value;
    tmmcomplex   *pc=   p2data.value;
    tmmint       *pi=   p2data.value;
    tmmpattern   *pp=   p2data.value;

 /* printf("fi %d%d%d" */
    /* 	   ,mmfatr.header.object,mmfatr.header.format,mmfatr.header.field); */
    char aline[nMAXLINE];unsigned int i=0;
    unsigned int i1,i2;
    //the call to the specific reading function..was there a simpler way??!!
#define rf (*p2reader[mmfatr.header.object]	\
	    [mmfatr.header.format]		\
	    [mmfatr.header.field])		\
      (aline,&i1,&i2,&v1,&v2);
    while((fgets(aline,nMAXLINE,pfile)!=NULL 
	   && iscomment(aline)==false
	   && isblank(aline)==false)
	  /* || i<len */ ){//todo could check if not enough data

#define iffield(x) if((mmfatr.header).field==indexof(x,field))
      //if(p2data.value!=NULL){//the simpler way:
	//..if i just read in a max of 4 strings, then i could have done the
	//..type conversions here w/o the use of a specific function
      iffield(REAL){        tmmreal    v1,v2; rf; pr[i]=v1;}
      else iffield(DOUBLE){ tmmdouble  v1,v2; rf; pd[i]=v1;}
      else iffield(COMPLEX){tmmreal    v1,v2; rf; pc[i]=v1+v2*I;}
      else iffield(INTEGER){tmmint     v1,v2; rf; pi[i]=v1;}
      else iffield(PATTERN){tmmpattern v1,v2; rf; pp[i]=true;}
	//}//old commnet:if field is pattern then nothing to do 
      //and already set to null
      //else{assert(p2data.value==NULL);}
      if(p2data.i1!=NULL){pi1[i]=i1;};if(p2data.i2!=NULL){pi2[i]=i2;}

      i++;}//<-the while loop closer
  }

  fclose(pfile);//really the caller should take reponsibility to close

return mmdata;
#undef iffield
#undef rf
#undef mmfatr
#undef p2data
}


static unsigned int putp2data(structmmfileattribs *attr, structp2data *p2data){

  //size the array
  unsigned int len;
  if ((&attr->header)->format==indexof(ARRAY,format))
{len=((&attr->size)->ncols)*((&attr->size)->nrows);}
  else {len=(&attr->size)->nonzeros;}

#define ma(typ,n) malloc(n*sizeof(typ))//should zero with calloc to be safer
  //pointers to index
  if ( (&attr->header)->object==indexof(VECTOR,object)){
    p2data->i2=NULL;//only 1-D
    if( (&attr->header)->format==indexof(COORDINATE,format)){
      p2data->i1=(tmmindex*) ma(tmmindex,len);}
//the casting doesn't do anything b/c i1 is void*
  }
  else if((&attr->header)->object==indexof(MATRIX,object)){
    if((&attr->header)->format==indexof(COORDINATE,format)){
      p2data->i1=(tmmindex*) ma(tmmindex,len);
      p2data->i2=(tmmindex*) ma(tmmindex,len);}
  }
  //..else it's array fmt and null i1 and i2
  if( (&attr->header)->format==indexof(ARRAY,format) )
    {p2data->i1=NULL;p2data->i2=NULL;}

  //pointers to values. 
#define iffield(x) if((&attr->header)->field==indexof(x,field))
#define p2valueistype(typ) {p2data->value= ma(typ,len);}
       iffield(REAL)    p2valueistype(tmmreal)
  else iffield(DOUBLE)  p2valueistype(tmmdouble)
  else iffield(COMPLEX) p2valueistype(tmmcomplex)
  else iffield(INTEGER) p2valueistype(tmmint)
  else iffield(PATTERN) p2valueistype(tmmpattern)
    // iffield(PATTERN) p2data->value=NULL;

return len;
#undef iffield
#undef ma
#undef p2valueis
}



static bool isblank(char *aline){
  //if (aline[0]=='\r' || aline[0]=='\n'){return true;}
  int c=0;unsigned int lenline=strlen(aline);
  while(c<lenline){
    if (aline[c]=='\t' || aline[c]==' '){c=c+1;continue;}
    else if (aline[c]=='\n' || aline[c]=='\r'){return true;}
    return false;//non-whitespace char encountered
  }
  printf("should never be here");
  assert(0);
  ;return false;
}



static bool iscomment(char *aline){ //or garbage
  if (strchr(aline,'%')!=NULL){return true;}
  //  if (isdigit(aline[0])==false){return true;}
 else{return false;}
}

static bool isheader(char *aline){
  //%%MatrixMarket...etc must be flush
  char *ps=strstr(aline,strmmid);
  if (ps==NULL){return false;}
  if (ps==aline){return true;}
  else{return false;}
}

static int putheaderinfo(char *aline, structheader *header){
/*header format is %%MatrixMarket object format field symmetry
object:    matrix or vector
format:    coordinate or array
field:     real, double, complex, integer, or pattern
symmetry:  general (legal for real, complex, integer, or pattern fields)
           symmetric (for real, complex, integer, or pattern)
           skew-symmetric (real, complex, or integer)
	   hermitian (complex)*/
  char ot[WLEN],ft[WLEN],fd[WLEN],sy[WLEN];
  int n;
  n= sscanf(aline,"%*s %s %s %s %s",/*dontcare,*/ot, ft, fd,sy);
  header->object=indexof(ot,object);
  header->format=indexof(ft,format);
  header->field=indexof(fd,field);
  header->symmetry=indexof(sy,symmetry);
  return n;//returns number of matches. should=4
}


//i want to return multiple values easily
//i either have to use ptrs or i have to make a struct??
static int putsizeinfo(char *aline,structsize *size,structheader *header){
  /*for array format, size line has: m n
    for coordiante format size line has: m n nonzeros*/
  unsigned int first,second,third;
  int  nsizelineelems= sscanf(aline,"%d %d %d",&first,&second,&third);

#define ifcase(obj,fmt) if						\
  (header->object==indexof(obj,object) &&			\
   header->format==indexof(fmt,format))
#define sizelineis(frows,fcols,fnonzeros) \
  {size->nrows=frows;					     \
    size->ncols=fcols;					     \
    size->nonzeros=fnonzeros;}

  ifcase(MATRIX,COORDINATE)   sizelineis(first,second,third)
  else ifcase(MATRIX,ARRAY)      sizelineis(first,second,0)
  else ifcase(VECTOR,COORDINATE) sizelineis(first,1,second)
  else ifcase(VECTOR,ARRAY)      sizelineis(first,1,0)
  else{assert(0);}

return nsizelineelems;
#undef ifcase
#undef sizelineis 
}



// want to implement error codes
static unsigned int putfileattribs(FILE *pfile, structmmfileattribs *mmfattribs){
  /*gets file attributes and checks for errors in metadata
    returns error codes. 0 means no problem
    check error code before using mmfattribs
  */

  char aline[nMAXLINE];//should be 'blanked' to be safe?

  bool didheaderline=false,didsizeline=false;
  while(//fgets(aline,nMAXLINE,pfile)!=NULL &&
  (didheaderline==false || didsizeline==false)
    ){
  if (fgets(aline,nMAXLINE,pfile)==NULL){break;}
  if (isblank(aline)==true){continue;}

  //process header line
  if(didheaderline==false){
    if (isheader(aline)==true){
      int nhdr=putheaderinfo(aline,&mmfattribs->header);
      if(nhdr!=4){return 1;}//"bad header line";
      else{ //the header fields are filled but check contents
	//todo: macro the verbosity
	if ( (indexof(MATRIX,object)==mmfattribs->header.object)  ||
	     (indexof(VECTOR,object)==mmfattribs->header.object)){/*no problem*/}
	else{return 11;}//unknown object
	if ( (indexof(COORDINATE,format)==mmfattribs->header.format) ||
	     (indexof(ARRAY,format)==mmfattribs->header.format)){}
	else {return 12;}//unknown format
	if ( (indexof(REAL,field)==mmfattribs->header.field) ||
	     indexof(DOUBLE,field)==mmfattribs->header.field ||
	     indexof(COMPLEX,field)==mmfattribs->header.field ||
	     indexof(INTEGER,field)==mmfattribs->header.field ||
	     indexof(PATTERN,field)==mmfattribs->header.field){}
	else{return 13;}//unknown field
	if ( indexof(GENERAL,symmetry)==mmfattribs->header.symmetry ||
	     indexof(SYMMETRIC,symmetry)==mmfattribs->header.symmetry ||
	     indexof(SKEWSYMMETRIC,symmetry)==mmfattribs->header.symmetry ||
	     indexof(HERMITIAN,symmetry)==mmfattribs->header.symmetry){}
	else{return 14;}//unknown symmetry
	didheaderline=true;}
    }
  }

  //process size line
  if(didsizeline==false){
  if (iscomment(aline)==false){//then it's a size line
  int nsle=putsizeinfo(aline,&mmfattribs->size,&mmfattribs->header);
  if ( (nsle<=0) || (nsle>3) )
    {return 2;}//bad size line}
  if (  (nsle==2) &&
    ( indexof("coordinate",format)==mmfattribs->header.format))//vs coord
    {return 23;}// missing number of nonzeros in size line
	    
  didsizeline=true;
  //if nsle=3 and format=array, then just ignore (3rd) nonzero field
  //if nsle=1 then it's a vector, ignore other two nonzero fields	
}
}
      
}

  if(didheaderline==false){return 10;}//no header line found
  if (didsizeline==false){return 20;}//no size line found
  return 0;
}


//
//don't forget to free malloc'd
//an idea is to make a OO wrapper on top of the above
