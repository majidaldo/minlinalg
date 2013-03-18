/*
reads matrix market format files
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for str funcs like strcmp
#include "mmtypes.h"
#include "utils.h"
#include "readingfuncs.h"
#include "readingfuncptrs.h"
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
const char *strmmid="%%MatrixMarket";
#define WLEN 30
/*the above line makes WLEN compile-time constant
  const int wlen=123 is not compile-time constant which
  generates an error originating in the struct definition
  why? idk*/
//const int WLEN=30;//"word length"

typedef struct {unsigned int object,format,field,symmetry;}
  structheader;
typedef struct {unsigned long int nrows, ncols , nonzeros;}
  structsize;
typedef struct {structheader header; structsize size;}
  structmmfileattribs ;

//checking utilities
static bool iscomment(char *aline);
//why do i have to declare prototype THEN define??
//static: only visibile in this file's scope
static bool isheader(char *aline);
static bool isblank(char *aline);

//1. read file info
//lower level
static int putheaderinfo(char *aline, structheader *header);
static int putsizeinfo(char *aline, structsize *size);
//higher level
unsigned int putfileattribs(FILE *pfile,structmmfileattribs *mmfattribs);

//a datatype for the following function
typedef struct {void *i1,*i2,*value;} p2data;//add ptr to reading func?

//function to return masked pointers so rename?
static p2data *returnpointers2data(structheader *header,p2data *p2data);



//todo the command line checks the file
int main(){



/*   FILE *pfile=fopen("test.mm","r"); */
/*   structmmfileattribs mmfatr; */

/*   if (pfile==NULL){ */
/*     printf("couldn't open file \n"); */
/*     exit(1); */
/*   } */
/*   else{ */
/*     /\* printf("file open success\n"); *\/ */
/*     unsigned int aerr=putfileattribs(pfile,&mmfatr); */
/*     /\* printf("\n nz %d%",mmfatr.size.nonzeros); *\/ */
/*     printf("\n attribs errorc: %d \n",aerr); */
/*     char aline[nMAXLINE]; */
/*     while(fgets(aline,nMAXLINE,pfile)!=NULL){printf(aline);} */
/*     } */
/* fclose(pfile); */

  // typedef int (*tgenericfp)();//func ptr that takes no args
  int (*fptrs[30])();
  //fptrs[0][0][0]=(tgenericfp)read000;//cast a func to generic so i can assign it
  /*to cast to appropriate sig*/

  //make a func readany with all cases taking all 4 args ptrs plus
  //..it typecasts first before going to case

  unsigned int i1;void *i2=NULL;float v1;void *v2=NULL ;
  typedef int (*ra000)(char *l,unsigned int *i1,float *v1);
  (*p2reader[0][0][0])("11 3.14",&i1,&i2,&v1,&v2);//pts to read000
  printf("%d %f",i1,v1);

return 0;}

/*need to declare data!! not just point to it*/
static p2data *returnpointers2data(structheader *header,p2data *p2data){

  //pointers to index
  if ( header->object==indexof(VECTOR,object)){p2data->i2=NULL;//only 1-D
    if(header->format==indexof(COORDINATE,format)){(tmmindex*) p2data->i1;}
//is this all i need to say to typecast?
  }
  if(header->object==indexof(MATRIX,object)){
    if(header->format==indexof(COORDINATE,format)){
      (tmmindex*) p2data->i1;(tmmindex*) p2data->i2;}
  }

  //pointers to values
#define iffield(x) if(header->field==indexof(x,field))
  iffield(REAL)    (tmmreal*) p2data->value;
  iffield(DOUBLE)  (tmmdouble*) p2data->value;
  iffield(COMPLEX) (tmmcomplex*) p2data->value;
  iffield(INTEGER) (tmmint*) p2data->value;
  iffield(PATTERN) p2data->value=NULL;
#undef iffield

}





static bool isblank(char *aline){
  //if (aline[0]=='\r' || aline[0]=='\n'){return true;}
  int c=0;unsigned int lenline=strlen(aline);
  while(c<lenline){
    if (aline[c]=='\t' || aline[c]==' '){c=c+1;continue;}
    else if (aline[c]=='\n' || aline[c]=='\r'){return true;}
    return false;//non-whitespace char encountered
  }
  //printf("should never be here")
  ;return false;
}


//redundancy. why do i have to match what i declared before main?
static bool iscomment(char *aline){ //or garbage
  //if (strchr(aline,'%')!=NULL){return true;}
  if (isdigit(aline[0])==false){return true;}
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
/*
header format is %%MatrixMarket object format field symmetry
object:    matrix or vector
format:    coordinate or array
field:     real, double, complex, integer, or pattern
symmetry:  general (legal for real, complex, integer, or pattern fields)
           symmetric (for real, complex, integer, or pattern)
           skew-symmetric (real, complex, or integer)
	   hermitian (complex)
*/

  //char dontcare[WLEN];//matrixmarket word
  char ot[WLEN],ft[WLEN],fd[WLEN],sy[WLEN];
  int n;
  n= sscanf(aline,"%*s %s %s %s %s",/*dontcare,*/ot, ft, fd,sy);
  header->object=indexof(ot,object);
  header->format=indexof(ft,format);
  header->field=indexof(fd,field);
  header->symmetry=indexof(sy,symmetry);
  return n;//returns number of matches. should=5
}


//i want to return multiple values easily
//i either have to use ptrs or i have to make a struct??
static int putsizeinfo(char *aline,structsize *size){
  /*for array format, size line has: m n
    for coordiante format size line has: m n nonzeros*/

  int  nsizelineelems= sscanf(aline,"%d %d %d"
			      ,&size->nrows
			      ,&size->ncols
			      ,&size->nonzeros);
  return nsizelineelems;
//should return 2 if array. 3 for coordinate
}



// want to implement error codes
unsigned int putfileattribs(FILE *pfile, structmmfileattribs *mmfattribs){
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
	if(nhdr!=5){
	  return 1;//"bad header line";
	}
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
	int nsle=putsizeinfo(aline,&mmfattribs->size);
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

//def readarray dataline
//def readcoord dataline

//def getattrib func
//def chk for consistency
//def  readnextdataline

//chk m*n data lines for value
//fx to get metadata

//skip blank lines
//close file
//check for file open errors


//an idea is to make a OO wrapper on top of the above
