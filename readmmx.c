/*
reads matrix market format files
*/

//todo if not included include
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for str funcs like strcmp


#include "mmtypes.h"
//eh no need. include mmtypes.def which defines data structs and 
//assoc  funcs

//nice. no function depends on a global variable :)
//the design is modular. every function is given a context.
//..the function does not push back
//se stderr
//todo enum errors

#define VECTOR "vector"
#define MATRIX "matrix"
//format
#define COORDINATE "coordinate"
#define ARRAY "array"
//field
#define REAL "real"
#define DOUBLE "double"
#define COMPLEX "complex"
#define INTEGER "integer"
#define PATTERN "pattern"
//symmetry
#define GENERAL "general"
#define SYMMETRIC "symmetric"
#define SKEWSYMMETRIC "skew-symmetric"
#define HERMITIAN "hermitian"

char *object[]={VECTOR, MATRIX};
char *format[]={COORDINATE, ARRAY};
char *field[]={REAL, DOUBLE, COMPLEX, INTEGER, PATTERN};
char *symmetry[]={GENERAL, SYMMETRIC, SKEWSYMMETRIC, HERMITIAN};

typedef struct {unsigned int object,format,field,symmetry;}
  structheader;
typedef struct {unsigned long int nrows, ncols , nonzeros;}
  structsize;
typedef struct {structheader header; structsize size;}
  structmmfileattribs ;



#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))


//nice to make a logical errors table
//.. could use switch statement to id

//checking utilities
static bool iscomment(char *aline);
//why do i have to declare prototype THEN define??
//static: only visibile in this file's scope
static bool isheader(char *aline);
static bool isblank(char *aline);
static int indexof(char *aword, char *arrayofwords[]);


//1. read file info
//lower level
static int putheaderinfo(char *aline, structheader *header);
static int putsizeinfo(char *aline, structsize *size);
//higher level
unsigned int putfileattribs(FILE *pfile,structmmfileattribs *mmfattribs);

//a datatype for the following function
typedef struct {void *i1,*i2,*value;} p2data;
static p2data *returnpointerarray(structheader *header,p2data *p2data);

//combos of header fields correspond to a dataline dtype
//which i will make a struct. make an array of them
//then 


/* //2. reading */
/* static void readdataline(structheader *headerattribs); */
//malloc the dtype?
//use void ptrs to a determined dtype output
//structmmfileattribs *mmfattribs determines -> dtype
//static void *extractdataline(ptrdtype,char *aline);
//static void 

//define MMATRIX to be evaluate strcomp
//associate a number with all combos

//list of dtypes and myfunctions corresponding to datalines
//list of myfunctions(scanf(aline)) that could be pointed to
//header -> myfunctions (scanf call -> into data struct
//  func(header)
//value field is common 
//list scanning fncs when i call the below i already know types
//readnlines(header,pfile,n)if real then:
//   realreader(aline,*realdatastruc){scanf("%d %d",datastructmmbrs=>)}
//gives back num of elems which can be used to chk.
//return scanning function (header, 

//1. def howtoreadline(mmfileattribs) returns ptrs to
//specific funcs, and data structs struct {void *ptrtofun, void *ptrtodtype}
//2. then just have a generic readline(aline,ptrtofunc,ptrtodatastruct)



//if i wanted to read past


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

  printf("%d" ,indexof("complesx" ,field));

return 0;}



static p2data *returnpointerarray(structheader *header,p2data *p2data){

  //pointers to index
  if ( header->object==indexof(VECTOR,object)){p2data->i2=NULL;//only 1-D
    if(header->format==indexof(COORDINATE,format)){(tmmindex) p2data->i1;}
//is this all i need to say to typecast?
  }
  if(header->object==indexof(MATRIX,object)){
    if(header->format==indexof(COORDINATE,format)){
      (tmmindex) p2data->i1;(tmmindex) p2data->i2;}
  }

  //pointers to values
  switch(header->field){
  case (indexof(REAL,field)):  (tmmreal) p2data->value;break;
  case indexof(DOUBLE,field):  (tmmdouble) p2data->value;break;
  case indexof(COMPLEX,field): (tmmcomplex) p2data->value;break;
  case indexof(INTEGER,field): (tmmint) p2data->value;break;
  case indexof(PATTERN,field): p2data->value=NULL;break;
  }

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

  char dontcare[WLEN];//matrixmarket word
  char ot[WLEN],ft[WLEN],fd[WLEN],sy[WLEN];
  int n;
  n= sscanf(aline,"%s %s %s %s %s",dontcare,ot, ft, fd,sy);
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
	  if ( (indexof(MATRIX,object)==mmfattribs->header.object)  ||
	       (indexof(VECTOR,object)==mmfattribs->header.object) ){/*no problem*/}
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


static int indexof(char *aword, char *arrayofwords[]){
  int i; 
  for (i=0 ; i<COUNT_OF(*arrayofwords) ;i++){
    if (strcmp(aword,arrayofwords[i])==0){return i;}}
  return -1;//not found
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
