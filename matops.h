#ifndef MATOPS_H
#define MATOPS_H

#include <stdio.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>

/*this serves to abstract away the matrix representation
i just care about the index

this really needs an object-oriented approach to handle all cases because you
have the following properties for matrices:
1. data type (computation)
2. matrix type data representation (computation)
3. mathematical type (math)
4. array operations  (computation)
5. matrix operations (math)

2 and 3 don't necessarily correspond. 4 and 5 don't necessarily correpond.

3 and 5, the math, can be separated out.

the process is like this. from the higher to lower level
1. matrix ops
1. matrix-matrix ops
2. array ops


*/





/*make row iter, col iter, diag iter. ..any iter. over matrix.
then ask if the pointed to contents the same? they are the same if the start and
end end "path" point to the same contents. if yes then elem2elem can be can be 
can be converted to onearrayelemtoonearrayelem. so then you can operate on any
two matrix representations.


make array struct that has start end and datatype

 */


//data types for the matrix
#define MATFLOAT    float
#define MATDOUBLE  double
#define MATCOMPLEX complex
#define MATINT     int
#define MATBOOL    bool



//attribs are a set of properties


/*i have a mapping from dense<-matrix types<-data representations
the arrow is pointing from unique elements*/
#define matrep attribs[0] //roughly corresponds to mathematical matrix types
typedef enum  { rowmjr, colmjr//dense (can convert b/w them) ...
		  //...in this program, can only write to these
                  //..but can "source" from the following
		  ,constmr
		  ,idmr
		  ,diagmr
		,matreplen} matrepnum;


#define matdtype attribs[1]
#define endt(TYPE) en##TYPE
//stands for enumerate data type
typedef enum { endt(MATFLOAT), endt(MATDOUBLE), endt(COMPLEX)
	       , endt(MATINT), endt(BOOL)
,dtypenumlen} dtypenum;
static size_t sizeofmatdtype[]={//match enum
sizeof(MATFLOAT),sizeof(MATDOUBLE),sizeof(MATCOMPLEX)
,sizeof(MATINT),sizeof(MATBOOL)};



//the functions to apply to the mapping
typedef enum {elemadd,elemmul,elemset
	      ,elemopnumlen} elemopnum; 
//these are the fundamental operations. compound these ops for application
typedef enum {one2one,acoloverrows,redrow,redcol,transposemap,redall
	      ,matmapnumlen} matmapnum;
typedef enum {matadd,matmul
	      ,matopnumlen} matopnum;


//i cant help but think of multidimensional arrays as just arrays in arrays
//but i will do matrix..yawn.
typedef struct structmatrix {
  void *data;//just an array
  unsigned int nrows,ncols;
  unsigned int attribs[3];//<-make sure to change this num with more attributes
  //dtypenum datatype;
  
  //struct structmatrix *parent;//a mat will have a parent if it uses the 
  //parent's data (but would not have the same *data/data[0]  member).
  //good to chk for overwriting
} structmatrix;
/*in this way if the dst op+mapping combo is incompatible with the src it will block. 
so you can  transpose two mats row/col to the same data (b/c it's just a view)
 but possibly other ops will block*/
//a transpose is setting 
//will brute force this by comparing unique iterators of both types.

/* typedef struct matrixiterator { */


//if i'm a view to another  matrix then block.




typedef  void* (*fpidx)(unsigned int *ri, unsigned int *ci, structmatrix *amat);
fpidx getidxingfunc(structmatrix *amat);
void *idxasrowmjr(unsigned int *ri, unsigned int *ci, structmatrix *rowmjrmat);
void *idxascolmjr(unsigned int *ri, unsigned int *ci, structmatrix *rowmjrmat);
void *idxasconstm(unsigned int *ri,unsigned int *ci, structmatrix *aconstmat);

void *overcol(unsigned int *cn, structmatrix *amat);
void *overrow(unsigned int *rn, structmatrix *amat);
void *overdiag(unsigned int *dn, structmatrix *amat);


//mathematical properties
static bool isquare(structmatrix *amat);
static bool issqdiag(structmatrix *amat);
static bool isconstant(structmatrix *amat);
static bool isdense(structmatrix *amat);
static bool isconstant(structmatrix *amat);

typedef void* (*fpop)(void*,void*);// fp to the operations 
//voids are src,dst datatypes mappings
static void *opone2one(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);
static void *oprows2cols(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);
//todo
static void *opondiagonal(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);
static void *oponlowertri(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);
static void *oponuppertri(structmatrix *srcmat, structmatrix *dstmat
		, fpop fpop);

//fill in the blanks
structmatrix creatematrix(void *datainmatrepform
			  ,unsigned int nrows,unsigned int ncols
			  ,dtypenum dtnum, matrepnum mrnum
);
//all matrices have parents except for the base

structmatrix copymatrix(structmatrix *src);//return is orphaned
//iter with uniqueiter



//when copying the new mat will have no parent. the data will copy.
//if startptr same as end ptr ..(assert other props the same) memcpy
void *freematrixdata(structmatrix *amat);//if im not the parent then ok

structmatrix convert2double(structmatrix *nondblmat);

//separate view from representoin

//this is different from transpose mapping
//views

//this thinking is just for dense special case
//views must be contain the same or subset of items

/* structmatrix submatrix(structmatrix *bigmat */
/* 		       ,unsigned int startri, unsigned int startci */
/* 		       ,unsigned int endri,   unsigned int endci);//inclusive */
/* //^gets rows, cols, subsqrs, and consts. set *parent */
//make this in terms of ops of src and dst matrices 
 
//init a mat will null


//are selected matrix elems the same (mappes to same memloc), unique, or contiguous?
//answer by looping throughh iterators,  if contiguous not unique


//all operations are defined as matrix-matrix operations. an operation
//is something that changes data in the destination
void *muldoubles(MATDOUBLE *src, MATDOUBLE *dst);
void *adddoubles(MATDOUBLE *src, MATDOUBLE *dst);
void *setdouble (MATDOUBLE *src, MATDOUBLE *dst);

static fpop getmatrixop(structmatrix *srcmat,structmatrix *dstmat, elemopnum opnum);

typedef void* (*fpmap)(structmatrix *srcmat, structmatrix *dstmat,fpop fpop);
static fpmap getmatrixmapping(structmatrix *srcmat,structmatrix *dstmat
			      ,matmapnum matmapnum);

void *arrayop(structmatrix *srcmat, structmatrix *dstmat
		 ,elemopnum opnum,matmapnum mapnum);
//returns data at dst place


//structmatrix matrixmatrixmul(structmatrix *A,structmatrix *B);
structmatrix matrixmatrixmuldbl(structmatrix *A,structmatrix *B); 


//the user interface. mathematical level
structmatrix matrixop(structmatrix *mat);
structmatrix matrixmatrixop(structmatrix *mat1,structmatrix *mat2,matopnum mopnum);





void *printmat(structmatrix *amat);


//attach iterator compatible with matrix
//each iterator is "1D". gets a list of coords. 

#endif
