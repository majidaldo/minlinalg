#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "mmtypes.h"
#include "utils.h"


void readingargs(char *sfargs20, char *ptrargs20, char *funcargs100
,  unsigned int object,  unsigned int format,  unsigned int field);


  //define unsigned int index2casenum[][][](do a +1 to reserve zero for unassigned
//in separate file

int main(){

  FILE *phead=fopen("readingfuncs.h","w");
  FILE *pimp=fopen("readingfuncs.c", "w");
  FILE *pfptrs=fopen("readingfuncptrs.h","w");

  fputs("#ifndef READINGFUNCS_H\n#define READINGFUNCS_H\n\n",phead);
  fprintf(pfptrs
 	  ,"int (*p2reader[%u][%u][%u])\
(char *aline,void *i1,void *i2,void *v1,void *v2)={"
	  ,COUNT_OF(object),COUNT_OF(format),COUNT_OF(field));
  fputs("#include \"readingfuncs.h\"\n",pimp);
  fputs("#include <string.h>\n\n",pimp);

  //fprintf(phead,"int (*preader[][][])()");

  /* char sfa[20], pta[20], far[50]; */
  /* readingargs(sfa,pta,far,0,0,4); */
  /* puts(sfa);puts(pta);puts(far); */

  char sfa[20], pta[20], far[100];//<-dont need this matches commented below
  char scanfstr[50], funcstr[150];
  unsigned int iob,ifo,ifi;
  for(iob=0;iob<COUNT_OF(object);iob++){
    for(ifo=0;ifo<COUNT_OF(format);ifo++){
      for(ifi=0;ifi<COUNT_OF(field);ifi++){
	//skip over nonsense combos
  	if (ifo==indexof(ARRAY,format) && ifi==indexof(PATTERN,field)){continue;}

  	readingargs(sfa,pta,far,iob,ifo,ifi);
	//declare in header
  	sprintf(funcstr,"read%d%d%d",iob,ifo,ifi);//enclosing fnc IDed by index
  	//fprintf(phead,"int %s(char *aline,%s)",funcstr,far);fputs(";\n",phead);
	fprintf(phead,"int %s(char *aline,void *i1,void *i2,void *v1,void *v2)"
		,funcstr);fputs(";\n",phead);
	//function pointer init
	fprintf(pfptrs,"\n[%u][%u][%u]=%s,",iob,ifo,ifi,funcstr);
  	//implement
 	sprintf(scanfstr,"return sscanf(aline,%s,%s)",sfa,pta);//scanf signature
	//fprintf(pimp,"\nint %s(char *aline,%s){\n%s;}",funcstr,far,scanfstr);
	fprintf(pimp,"int %s(char *aline,void *i1,void *i2,void *v1,void *v2){\n%s;}\n"
		,funcstr,scanfstr);
      }}}

  fputs("\n\n#endif",phead);fprintf(phead,"\n");fprintf(pfptrs,"};\n");
  fclose(phead);fclose(pimp);fclose(pfptrs);
  return 0;}


void readingargs(char *sfargs20, char *ptrargs20, char *funcargs100
,  unsigned int iobject,  unsigned int iformat,   unsigned int ifield){
  /*this function is impossible to follow. blame C.*/

  char sf1[20],sf2[20]; //str formatting. 1. for the index 2. datatype
  char p1[20],p2[20],p3[20],p4[20];//4 ptrs representing the ptr args to scanf
  char fa1[100],fa2[100],fa3[100],fa4[100];//function arg types

#define zeroindexes strcpy(sf1,"\"");			\
  strcpy(p1,"");strcpy(p2,"");				\
  strcpy(fa1,"");strcpy(fa2,"");
#define oneindex strcpy(sf1,"\"%u ");		\
  strcpy(p1,"i1,");strcpy(p2,"");		\
  strcpy(fa1,stmmindex);			\
  strcat(fa1," *i1,");strcpy(fa2,"");
#define twoindexes strcpy(sf1,"\"%u %u ");	\
  strcpy(p1,"i1,");strcpy(p2,"i2,");		\
  strcpy(fa1,stmmindex); strcpy(fa2,stmmindex);	\
  strcat(fa1," *i1,");strcat(fa2," *i2,");

  if(indexof(COORDINATE,format)==iformat){
    if(indexof(VECTOR,object)==iobject){oneindex}
    else{twoindexes} //else matrix
  }
  else{zeroindexes} //else it's an array which has no index matrix or vector

#define fielddtype(strfield) ctypes[indexof(strfield,field)]
#define iffield(typ) if(indexof(typ,field)==ifield)
#define dtypes(typ,msf,mp3,mp4) {strcpy(sf2,msf);		\
    strcpy(p3,mp3);strcpy(p4,mp4);				\
    if(p3[0]!='\0'){char tmp[25];strcpy(tmp,fielddtype(typ));	\
      strcat(tmp,p3);strcpy(fa3,tmp);}				\
    else{strcpy(fa3,"");}				\
    if(p4[0]!='\0'){char tmp[25];strcpy(tmp,fielddtype(typ));	\
      strcat(tmp,p4);strcpy(fa4,tmp);}				\
    else{strcpy(fa4,"");}}
  iffield(REAL) dtypes(REAL,"%f\"","v1,","" )
    iffield(DOUBLE) dtypes(DOUBLE,"%lf\"","v1,","")
    iffield(COMPLEX) dtypes(COMPLEX,"%f %f\"","v1,","v2")
    iffield(INTEGER) dtypes(INTEGER,"%d\"","v1,","")
    iffield(PATTERN) dtypes(PATTERN,"\"","","") 

    //put them all together
    strcpy(sfargs20,strcat(sf1,sf2));
  strcat(p1,strcat(p2,strcat(p3,p4)));
  strcat(fa1,strcat(fa2,strcat(fa3,fa4)));
  if(p1[strlen(p1)-1]==','){p1[strlen(p1)-1]='\0';}//trim last comma..
  // .. i thought i had to strlen-2??? S0 T1 R2 I3 N4 G5 ,6 \07 ???
  strcpy(ptrargs20,p1 );
  if(fa1[strlen(fa1)-1]==','){fa1[strlen(fa1)-1]='\0';}
  strcpy(funcargs100,fa1 );


#undef dtypes
#undef iffield
#undef zeroindexes
#undef oneindex
#undef twoindexes
}


