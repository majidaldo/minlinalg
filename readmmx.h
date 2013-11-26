#ifndef READMMX_H
#define READMMX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for str funcs like strcmp
#include "mmtypes.h"
#include "utils.h"
#include "readingfuncs.h"

#include <assert.h>
#include <stdint.h> //has datatype to store ptr as int



//..so can call indexed reader (*p2reader[][][])(i1,i2,v1,v2)
//..must make sure correct arg type

//nice. no function depends on a global variable :)
//the design is modular. every function is given a context.
//..the function does not push back
//se stderr
//todo enum errors
//nice to make a logical errors table
//.. could use switch statement to id

 
#define nMAXLINE  256 //why should i need to declare a limit??
static const char *strmmid="%%MatrixMarket";
#define WLEN 30
/*the above line makes WLEN compile-time constant
  const int wlen=123 is not compile-time constant which
  generates an error originating in the struct definition
  why? idk*/
//const int WLEN=30;//"word length"


//todo make the following readonly after the first assignment
typedef struct {
  unsigned int object,format,field,symmetry;}
  structheader;
typedef struct {
  unsigned long int nrows, ncols , nonzeros;}
  structsize;
typedef struct {
  structheader header;
  structsize size;}
  structmmfileattribs ;
typedef struct {tmmindex *i1 //index 1..could be null
,*i2; //index 2 second dimension..could be null
    void *value;//the value associated w/ index 1 and 2. 
} structp2data;
//..data here is malloced. remember to free
typedef struct {structp2data p2data;
  structmmfileattribs mmfileattribs;} structmmdata;

//checking utilities
static bool iscomment(char *aline);
//why do i have to declare prototype THEN define??
//static: only visibile in this file's scope
static bool isheader(char *aline);
static bool isblank(char *aline);

//1. read file info
//lower level
static int putheaderinfo(char *aline, structheader *header);
static int putsizeinfo(char *aline, structsize *size,structheader *header);
//higher level
static unsigned int putfileattribs(FILE *pfile,structmmfileattribs *mmfattribs);
static unsigned int putp2data(structmmfileattribs *attr,structp2data *p2data);
//highest level
//a common way to access data the way it was stored
structmmdata readmmfile(FILE *pfile);
void *getp2valueat(unsigned int *mmirow, unsigned int *mmicol, structmmdata *mmdata);




//todo make MM attribs const

//MM selection
//..gets row or column
//uintptr_t *
structpiterator getptrs2mmvec(unsigned int *mmirow,unsigned int *mmicol
			  ,structmmdata *mmdata);
typedef struct {void *array; unsigned int len;} structarray;

//printing
int printmmvalat(void *p2val, unsigned int* datatypenum);

//basic ops
/* void *oponvalat1(void *p2val, void* (*fptr)(void*) ); */
/* void *oponvalat2(void *p2val, void *p2opobj, void* (*fptr)(void*,void*) ); */
/* double *copyasdouble(unsigned int* mmirow,unsigned int* mmicol */
/* 		   ,structmmdata* p2mmdata); */

void *freemmdata(structmmdata *mmdata);



#endif
